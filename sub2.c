//
// subscriber with waitset and condition variable (blocking)
//

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
*/
#include "mb.h"                             // generated code

#define MY_TOPIC    "Voltage"               // DDS topic name
#define AND &&

enum { MAX_SAMPLES = 10 };                  // max num of sample for each take

// global variables
static dds_condition_t terminated_cond;     // terminated condition variable

// handle ctrl+c signal
static void sigint_handler (int sig)
{
    dds_guard_trigger (terminated_cond);    // set trigger_value
}

int main (int argc, char *argv[])
{
    // Change signal disposition
    struct sigaction sat;
    sat.sa_handler = sigint_handler;
    sigemptyset (&sat.sa_mask);
    sat.sa_flags = 0;
    sigaction (SIGINT, &sat, NULL);

    // Declare dds entities ------------------------
    dds_qos_t*          qos                 = NULL;
    dds_entity_t        domain_participant  = NULL;
    dds_entity_t        voltage_topic       = NULL;
    dds_entity_t        voltage_subscriber  = NULL;
    dds_entity_t        voltage_reader      = NULL;
    dds_condition_t     voltage_cond;      // condition variable

    dds_waitset_t       ws;
    dds_attach_t        ws_results[1];
    size_t              ws_result_size      = 1u;
    dds_time_t          ws_timeout          = DDS_SECS (10);

    void*               samples_ptr[MAX_SAMPLES];
    Modbus_voltage      samples[MAX_SAMPLES];
    dds_sample_info_t   samples_info[MAX_SAMPLES];
    uint32_t            sample_mask         = 0;

    // Initialize sample data
    memset (samples_ptr, 0, sizeof(samples_ptr));
    for (int i = 0; i < MAX_SAMPLES; i++)
    {
        samples_ptr[i] = &samples[i]; 
    }

    // Initialize DDS ------------------------
    int status = dds_init ( 0, NULL );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create domain participant
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
                &voltage_subscriber,    // pointer to created subscriber entity
                qos,                    // Qos on created subscriber (can be NULL)
                NULL                    // Listener on created subscriber (can be NULL)
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create reader with qos
    qos = dds_qos_create ();
    dds_qset_reliability (              // Set the reliability policy in the qos structure
        qos,                            // The pointer to the qos structure
        DDS_RELIABILITY_RELIABLE,       // kind: {RELIABLE, BEST_EFFORT}
        DDS_SECS (10)                   // max_blocking_time, default: 100ms
    );
    //dds_qset_history (qos, DDS_HISTORY_KEEP_ALL, 0);
    status = dds_reader_create (        // factory method to create typed reader
                voltage_subscriber,     // domain participant entity or subscriber entity
                &voltage_reader,        // pointer to created reader entity
                voltage_topic,          // topic entity
                qos,                    // Qos on created reader (can be NULL)
                NULL                    // Listener on created reader (can be NULL)
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);
    dds_qos_delete (qos);

    // Init condition variables
    terminated_cond = dds_guardcondition_create ();
    //or voltage_cond = dds_statuscondition_get (reader); 
    voltage_cond = dds_readcondition_create ( 
                    voltage_reader,     // Reader entity on which the condition is created
                    DDS_ANY_STATE       // mask set the sample_state, instance_state and view_state of the sample
                ); 

    // Attach condition variable to waitset
    ws = dds_waitset_create ();
    dds_status_set_enabled ( 
        voltage_reader,                 // Entity to enable the status
        DDS_DATA_AVAILABLE_STATUS       // mask Status value that indicates the status to be enabled
    );
    // attach cond
    status = dds_waitset_attach (
                ws,                     // pointer to a waitset
                voltage_cond,           // pointer to a condition to wait for the trigger value
                voltage_reader          // attach condition, could be used to know the reason for the waitset to unblock (can be NULL)
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);
    // attach terminated
    status = dds_waitset_attach (
                ws,                     // pointer to a waitset
                terminated_cond,        // pointer to a condition to wait for the trigger value
                terminated_cond         // attach condition, could be used to know the reason for the waitset to unblock (can be NULL)
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);


    // Waiting for writer appear -------------------------

    printf ("Waiting for writer...\n");
    status = dds_waitset_wait (
                ws, 
                ws_results, 
                ws_result_size, 
                DDS_INFINITY            // inf block
            ); 
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    while (!dds_condition_triggered (terminated_cond) )
    {
        // blocking
        status = dds_waitset_wait ( 
                    ws,                 // pointer to a waitset
                    ws_results,         // pointer to an array of attached_conditions based on the conditions associated with a waitset (can be NULL)
                    ws_result_size,     // number of attached conditions (can be zero)
                    ws_timeout          // timeout value associated with a waitset (can be INFINITY or some value)
                );
        DDS_ERR_CHECK (status, DDS_CHECK_REPORT);

        // timeout checking
        if (status == 0 )
        {
            printf ("Timed out while waiting for an event.\n");
            break;
        }

        // num of signaled waitset conditions
        if (status > 0)
        {
            // take sample from dds context
            int sample_count = dds_take (
                                voltage_reader, // reader entity 
                                samples_ptr,    // (void **) array of pointers to samples (pointers can be NULL)
                                MAX_SAMPLES,    // max num of samples to read
                                samples_info,   // pointer to an array of dds_sample_info_t
                                sample_mask     // filter mask, [sample, view, instance state]
                              );
            DDS_ERR_CHECK (sample_count, DDS_CHECK_REPORT);

            // give chance to catch terminate signal
            for (int i = 0; !dds_condition_triggered (terminated_cond) AND i < sample_count; i++)
            {
                if (samples_info[i].valid_data) // valid sample from sample_info
                {
                    printf("read: %d, %f\n", samples[i].id, samples[i].val);
                }
            }
        }
    }

    printf ("Cleaning up...\n");

    status = dds_waitset_detach (ws, voltage_cond);
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);
    dds_condition_delete (voltage_cond);

    status = dds_waitset_detach (   // Disassociate the condition attached with a waitset
                ws,                 // pointer to a waitset
                terminated_cond     // pointer to a condition to wait for the trigger value
            );
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);
    dds_condition_delete (terminated_cond);
    
    status = dds_waitset_delete (ws);
    DDS_ERR_CHECK (status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    dds_entity_delete (domain_participant);
    dds_fini ();
    printf ("Finished.\n");
    exit(0);
}