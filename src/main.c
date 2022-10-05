//Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <syslog.h>
#include "uci_cfg.h"

//Request handler function declaration
int request_handler(netsnmp_mib_handler *handler,
                    netsnmp_handler_registration *reginfo,
                    netsnmp_agent_request_info *reqinfo,
                    netsnmp_request_info *requests);

//Declaration of desired OID
static oid variable_oid[] = {1, 3, 6, 1, 4, 1, 48690, 10, 1, 0};

//Function definitions

//Init funcition that registers the OID and assigns handler function to it
void init_str_variable(void){
    int rc = 0;
    netsnmp_handler_registration *reginfo = NULL;

    reginfo = netsnmp_create_handler_registration("example variable",
                                                  request_handler, variable_oid,
                                                  OID_LENGTH(variable_oid), HANDLER_CAN_RWRITE);
    
    rc = netsnmp_register_handler(reginfo);
    if(rc != SNMPERR_SUCCESS){
        syslog(LOG_ERR, "Can't register SNMP OID handler\n");
    }
}

//DeInit function that unregisters the OID
void deinit_variable(void){
    unregister_mib(variable_oid,OID_LENGTH(variable_oid));
}

//SNMP request callback function
int request_handler(netsnmp_mib_handler *handler,
                    netsnmp_handler_registration *reginfo,
                    netsnmp_agent_request_info *reqinfo,
                    netsnmp_request_info *requests){
    
    int rc = 0;
    char data_buff[BUFF_SIZE];

    switch(reqinfo->mode){
        //Case that handles snmpget requests
        case MODE_GET:

        rc = uci_parameter_get(data_buff);

        if(rc != 0){
            syslog(LOG_ERR, "Failed to parse cfg option\n");
            return rc;
        }

        rc = snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                                      (u_char *)data_buff, strlen(data_buff));
        if(rc != 0){
            syslog(LOG_ERR, "Failed to malloc\n");
            return rc;
        }
        break;

        //Cases that handle snmpset requests
        case MODE_SET_RESERVE1:

        if(requests->requestvb->type != ASN_OCTET_STR){
            syslog(LOG_ERR, "Wrong snmpset input type\n");
            return 1;
        }

        break;

        case MODE_SET_COMMIT:

        rc = uci_parameter_set((char *)requests->requestvb->val.string);
        if(rc != 0){
            syslog(LOG_ERR, "Failed to set cfg option\n");
            return rc;
        }

        break;
    }

    return rc;
}
