#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include "mb.h" // generated code

// global variables
static volatile int do_loop = 1;

// signal handler
static void int_handler(int dummy) 
{
    do_loop = 0;
}

int main(int argc, char *argv[])
{
    // Handle system signal
    signal(SIGINT, int_handler);
    signal(SIGTERM, int_handler);

    // Declare dds entities ------------------------
    int status;
    dds_qos_t*   qos            = NULL;
    dds_entity_t participant    = NULL;
    dds_entity_t writer_topic   = NULL;
    dds_entity_t publisher      = NULL;  
    dds_entity_t writer         = NULL;
    
    // Initialize DDS ------------------------
    status = dds_init(argc, argv);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create participant
    status = dds_participant_create(&participant, DDS_DOMAIN_DEFAULT, qos, NULL);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create a publisher
    status = dds_publisher_create(participant, &publisher, qos, NULL);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create topic for writer
    status = dds_topic_create(participant, &writer_topic, &Modbus_voltage_desc, "Voltage", NULL, NULL);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Create writer without Qos
    status = dds_writer_create(participant, &writer, writer_topic, NULL, NULL);
    DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);

    // Prepare samples  ------------------------
    srand((unsigned int)time(NULL));
    float a = 5.0;
    Modbus_voltage writer_msg;
    writer_msg.id = 3;

    while (do_loop) 
    {
        // dds write
        writer_msg.val = ((float)rand()/(float)(RAND_MAX)) * a;
        status = dds_write(writer, &writer_msg);
        DDS_ERR_CHECK(status, DDS_CHECK_REPORT | DDS_CHECK_EXIT);
        printf("write: %f\n", writer_msg.val);
        dds_sleepfor (DDS_SECS(1));
    }

    printf("Sanitize\n");
    dds_entity_delete(participant);
    dds_fini();
    exit(0);
}