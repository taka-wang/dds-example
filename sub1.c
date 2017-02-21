#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include "mb.h" // generated code

#define MAX_SAMPLES 1

// global variables
static volatile int do_loop = 1;

// signal handler
static void int_handler(int dummy) 
{
    do_loop = 0;
}

static void data_available_handler(dds_entity_t reader) 
{
    void* samples[MAX_SAMPLES];
    dds_sample_info_t info[MAX_SAMPLES];
    
    uint32_t mask = 0;
    mask = DDS_ALIVE_INSTANCE_STATE;

    Modbus_voltage sample;
    samples[0] = &sample;
    
    // take!
    int ret = dds_take(reader, samples, MAX_SAMPLES, info, mask);
    if (DDS_ERR_CHECK(ret, DDS_CHECK_REPORT)) 
    {
        printf("GOT: %d, %f\n", sample.id, sample.val);
    }
}

int main(int argc, char *argv[])
{
    // Handle system signal
    signal(SIGINT, int_handler);
    signal(SIGTERM, int_handler);

    // Declare dds entities ------------------------
    int status;
    dds_qos_t*   qos                = NULL;
    dds_entity_t participant        = NULL;
    dds_entity_t reader_topic       = NULL;
    dds_entity_t subscriber         = NULL;
    dds_entity_t reader             = NULL;
    dds_readerlistener_t listener;
    
    // Initialize DDS ------------------------
    status = dds_init(argc, argv);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create participant
    status = dds_participant_create(&participant, DDS_DOMAIN_DEFAULT, qos, NULL);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create reader topic
    status = dds_topic_create(participant, &reader_topic, &Modbus_voltage_desc, "Voltage", NULL, NULL);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create a subscriber
    status = dds_subscriber_create(participant, &subscriber, qos, NULL);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // data available callback
    memset(&listener, 0, sizeof (listener));
    listener.on_data_available = data_available_handler;
    
    // Create reader with DATA_AVAILABLE status condition enabled
    qos = dds_qos_create();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (1));
    dds_qset_history (qos, DDS_HISTORY_KEEP_ALL, 0);

    status = dds_reader_create(subscriber, &reader, reader_topic, qos, &listener);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);
    dds_qos_delete (qos);


    while (do_loop) 
    {
        dds_sleepfor (DDS_MSECS(10));
    }

    printf("Sanitize\n");
    dds_entity_delete(participant);
    dds_fini();
    exit(0);
}