/**
 * nzedb-backend
 * Copyright (c) 2012-2014 Matthew Goff <matt@goff.cc>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 **/
#include "h/includes.h"
#include "h/main.h"

#include "h/dbconn_mysql.h"
#include "h/list.h"

using namespace std;

Main::Global* g_global; /**< Global variables. */

struct ThreadData
{
    string args;
    int sleep_h;
    int sleep_m;
    int sleep_s;
};

const int compute_seconds( const ThreadData* data );
//const void spawn_thread( ThreadData* data );
//void* process_request( void* input );
chrono::high_resolution_clock::time_point time_current;

// Eventually split this out to a config file and parse in nZEDb config files
const vector<ThreadData> thread_data
{
    { "php update_binaries.php", 0, 1, 0 },
    { "php update_releases.php 1 true", 0, 2, 0 },
    { "php postprocess.php all true", 0, 3, 0 },
    { "php predbftmatch.php full show", 0, 4, 0 },
    { "php requestid.php full show", 0, 5, 0 },
    { "php ../testing/Release/fixReleaseNames.php 1 true all yes", 0, 6, 0 },
    { "php ../testing/Release/fixReleaseNames.php 3 true other yes", 0, 6, 0 },
    { "php ../testing/Release/fixReleaseNames.php 5 true other yes", 0, 6, 0 },
    { "php ../testing/Release/removeCrapReleases.php true 2", 1, 30, 0 },
    { "php optimize_db.php run", 1, 0, 0 },
    //{ "php update_tvschedule.php", 60 * 60 * 24 },
    //{ "php update_theaters.php", 60 * 60 * 24 }
};

int main( const int argc, char* argv[] )
{
    UFLAGS_DE( flags );

    // Initialize global variables
    // Ensure globals are first as other items depend on them
    g_global = new Main::Global();

    if ( argc > 1 )
        Main::Startup( argv[1] );
    else
        Main::Startup();

    while ( !g_global->m_shutdown )
        Main::Update();
    // Fork to the background immediately to avoid shell output
    // daemon( 1, 0 );
/*
    ThreadData* data;
    ThreadData td;
    vector<ThreadData>::const_iterator vi;

    for ( vi = thread_data.begin(); vi != thread_data.end(); vi++ )
    {
        td = *vi;

        data = new ThreadData;
        data->args = td.args;
        data->sleep_h = td.sleep_h;
        data->sleep_m = td.sleep_m;
        data->sleep_s = td.sleep_s;

        spawn_thread( data );
    }

    pause();
*/
    // Cleanup the MySQL connector
    mysql_library_end();

    return 0;
}

/**
 * @brief Start the nzedb-backend server.
 * @param[in] config An optional path to a configuration file to load.
 * @retval void
 */
const void Main::Startup( const string& config )
{
    UFLAGS_DE( flags );
    g_global->m_shutdown = false;

    LOGFMT( 0, "%s started.", CFG_STR_VERSION );

    // This needs to be called prior to any threads firing off that may hit the DB
    if ( mysql_library_init( 0, NULL, NULL ) )
    {
        LOGSTR( flags, "Failed to initialize MySQL connector." );
        ::exit( EXIT_FAILURE );
    }

    for ( auto i = 0; i < CFG_MEM_MAX_DBCONN; i++ )
        new DBConnMySQL( DBCONN_TYPE_MYSQL, "localhost", "/var/run/mysqld/mysqld.sock", "nzedb", "nzedb", "nzedb" );

    // Wait for the threads to all initialize otherwise to ensure the update loop doesn't poll
    // them before they are ready
    while ( dbconn_list.size() < CFG_MEM_MAX_DBCONN )
        ::usleep( CFG_THR_SLEEP );

    return;
}

/**
 * @brief The core update loop of nzedb-backend. This loop spawns all other subsystem update routines and then sleeps for #CFG_THR_SLEEP each cycle.
 * @retval void
 */
const void Main::Update()
{
    g_global->m_time_current = chrono::high_resolution_clock::now();

    // Poll all database connectors
    Main::PollDBConn();

    // Sleep
    ::usleep( CFG_THR_SLEEP );

    return;
}

/**
 * @brief Polls all DBConn objects to ensure validity and process updates.
 * @retval void
 */
const void Main::PollDBConn()
{
    UFLAGS_DE( flags );
    ITER( vector, DBConn*, vi );
    DBConn* db;

    for ( vi = dbconn_list.begin(); vi != dbconn_list.end(); vi = g_global->m_next_dbconn )
    {
        db = *vi;
        g_global->m_next_dbconn = ++vi;

        if ( db->gStatus() == DBCONN_STATUS_READY || db->gStatus() == DBCONN_STATUS_NONE || db->gStatus() == DBCONN_STATUS_BUSY )
            continue;
        else if ( db->gStatus() == DBCONN_STATUS_ERROR )
        {
            LOGSTR( flags, "DBConn::MySQL::New()-> error while attempting to connect" );
            delete db;
            continue;
        }
        else if ( db->gStatus() == DBCONN_STATUS_CLOSE )
        {
            LOGSTR( flags, "DBConn::MySQL::New()-> connector closing down" );
            delete db;
            continue;
        }
    }

    return;
}

/**
 * @brief Constructor for the Main::Global class.
 */
Main::Global::Global()
{
    m_next_dbconn = dbconn_list.begin();
    m_shutdown = true;
    m_time_current = chrono::high_resolution_clock::now();

    return;
}

/**
 * @brief Destructor for the Main::Global class.
 */
Main::Global::~Global()
{
    return;
}

const int compute_seconds( const ThreadData* data )
{
    int hours = data->sleep_h * 60 * 60;
    int minutes = data->sleep_m * 60;
    int seconds = data->sleep_s;

    return hours + minutes + seconds;
}
/*
const void spawn_thread( ThreadData* data )
{
    pthread_t res_thread;
    pthread_attr_t res_attr;

    pthread_attr_init( &res_attr );
    pthread_attr_setdetachstate( &res_attr, PTHREAD_CREATE_DETACHED );
    pthread_create( &res_thread, &res_attr, &process_request, data );
    pthread_attr_destroy( &res_attr );

    return;
}

void* process_request( void* input )
{
    ThreadData* data = static_cast<ThreadData*>( input );

    system( data->args.c_str() );
    sleep( compute_seconds( data ) );
    spawn_thread( data );

    return 0;
}
*/
