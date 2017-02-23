//
// subscriber with listener (async)
//

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
*/
#include "mb.h"                     // generated code

#define MY_TOPIC        "Voltage"   // DDS topic name
#define MAX_SAMPLES     1           // max num of sample for each take

// global variables
static volatile int do_loop = 1;

// signal handler
static void int_handler (int dummy) 
{
    do_loop = 0;
}

static void data_available_handler (dds_entity_t voltage_reader) 
{
    void* samples[MAX_SAMPLES];
    dds_sample_info_t info[MAX_SAMPLES];
    
    uint32_t mask = 0;
    mask = DDS_ALIVE_INSTANCE_STATE;

    Modbus_voltage sample;
    samples[0] = &sample;
    
    // take!
    int ret = dds_take ( 
                voltage_reader,     // reader entity
                samples,            // (void **) array of pointers to samples (pointers can be NULL)
                MAX_SAMPLES,        // max num of samples to read
                info,               // pointer to an array of dds_sample_info_t
                mask                // filter mask, [sample, view, instance state]
            );
    if ( DDS_ERR_CHECK (ret, DDS_CHECK_REPORT)) 
    {
        printf ("read: %d, %f\n", sample.id, sample.val);
    }
}

int main (int argc, char *argv[])
{
    // Handle system signal
    signal (SIGINT,  int_handler);
    signal (SIGTERM, int_handler);

    // Declare dds entities ------------------------
    int status;
    dds_qos_t*   qos                = NULL;
    dds_entity_t domain_participant = NULL;
    dds_entity_t voltage_topic      = NULL;
    dds_entity_t subscriber         = NULL;
    dds_entity_t voltage_reader     = NULL;
    dds_readerlistener_t listener;
    memset (&listener, 0, sizeof(listener));
    
    // Initialize DDS ------------------------
    status = dds_init (0, NULL);
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create participant
    status = dds_participant_create (   // factory method to create domain participant
                &domain_participant,    // pointer to created domain participant entity
                DDS_DOMAIN_DEFAULT,     // domain id (DDS_DOMAIN_DEFAULT = -1)
                qos,                    // Qos on created domain participant (can be NULL)
                NULL                    // Listener on created domain participant (can be NULL)
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create reader topic
    status = dds_topic_create (         // factory method to create topic
                domain_participant,     // domain participant entity
                &voltage_topic,         // pointer to created topic entity
                &Modbus_voltage_desc,   // pointer to IDL generated topic descriptor
                MY_TOPIC,               // name of created topic
                NULL,                   // Qos on created topic (can be NULL)
                NULL                    // Listener on created topic (can be NULL)
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create a subscriber
    status = dds_subscriber_create (    // factory method to create subscriber
                domain_participant,     // domain participant entity
                &subscriber,            // pointer to created subscriber entity
                qos,                    // Qos on created subscriber (can be NULL)
                NULL                    // Listener on created subscriber (can be NULL)
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // data available callback
    listener.on_data_available = data_available_handler;
    
    // Create reader with DATA_AVAILABLE status condition enabled
    qos = dds_qos_create ();            // Allocate memory and initializes to default values for qos
    dds_qset_reliability (              // Set the reliability policy in the qos structure
        qos,                            // The pointer to the qos structure
        DDS_RELIABILITY_RELIABLE,       // kind: {RELIABLE, BEST_EFFORT}
        DDS_SECS (1)                    // max_blocking_time, default: 100ms
    );

    dds_qset_history (                  // Set the history policy in the qos structure.
        qos,                            // The pointer to the qos structure
        DDS_HISTORY_KEEP_ALL,           // kind: {KEEP_LAST, KEEP_ALL}
        0                               // depth: keep last depth, default: 1
    );

    status = dds_reader_create (        // factory method to create typed reader
        subscriber,                     // domain participant entity or subscriber entity
        &voltage_reader,                // pointer to created reader entity
        voltage_topic,                  // topic entity
        qos,                            // Qos on created reader (can be NULL)
        &listener                       // Listener on created reader (can be NULL)
    );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);
    dds_qos_delete (qos);


    while (do_loop) 
    {
        dds_sleepfor (DDS_MSECS(10));
    }

    // release resources
    printf ("Sanitize\n");
    dds_entity_delete (domain_participant);
    dds_fini ();
    exit (0);
}