#include "uci_cfg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uci.h>
#include <syslog.h>

int uci_parameter_get(char *buff){
    int rc = 0;
    struct uci_context *c = NULL;
    struct uci_package *pckg = NULL;
    struct uci_section *sec = NULL;
    struct uci_option *opt;

    c = uci_alloc_context();
    if(c == NULL){
        syslog(LOG_ERR, "Failed to allocate context(NETSNMP-MOD-DYNAMIC)\n");
        rc = 1;
        return rc;
    }

    rc = uci_load(c,CFG_NAME,&pckg);
    if(rc != 0 || pckg == NULL)
    {
        syslog(LOG_ERR, "Failed to load config(NETSNMP-MOD-DYNAMIC)\n");
        uci_free_context(c);
        rc = 1;
        return rc;
    }

    sec = uci_lookup_section(c,pckg,CFG_SECTION);
    if(sec == NULL)
    {
        syslog(LOG_ERR, "Failed to lookup section(NETSNMP-MOD-DYNAMIC)\n");
        rc = 1;
        goto PARAMETER_GET_ERR;
    }

    opt = uci_lookup_option(c,sec,CFG_OPTION);
    if(opt == NULL)
    {
        syslog(LOG_ERR, "Failed to lookup option(NETSNMP-MOD-DYNAMIC)\n");
        rc = 1;
        goto PARAMETER_GET_ERR;
    }

    if(!opt || opt->type != UCI_TYPE_STRING)
    {   
        syslog(LOG_ERR, "Parsed option is not a string!(NETSNMP-MOD-DYNAMIC)\n");
        rc = 1;
        goto PARAMETER_GET_ERR;
    }

    strncpy(buff, opt->v.string, strlen(opt->v.string));
    buff[strlen(opt->v.string)] = '\0';

    uci_unload(c,pckg);
    uci_free_context(c);
    return rc; 

    PARAMETER_GET_ERR:
    uci_free_context(c);
    uci_unload(c,pckg);
    return rc;
}

int uci_parameter_set(char *data){
    int rc = 0;
    struct uci_ptr config;
    struct uci_context* c;

    c = uci_alloc_context();
    if (!c) {
        syslog(LOG_ERR, "Failed to allocate context(NETSNMP-MOD-DYNAMIC set request)\n");
        rc = 1; 
        return rc;
    }

    char section_name[] = CFG_NAME_SECTION;

    rc = uci_lookup_ptr(c, &config, section_name, true);
    if (rc != UCI_OK || !config.s) {
        syslog(LOG_ERR, "Failed to lookup ptr(NETSNMP-MOD-DYNAMIC set request)\n");
        rc = 1;
        goto PARAMETER_SET_ERR;
    }

    config.option = CFG_OPTION;
    config.value = data;

    rc = uci_set(c, &config); 
    if (rc != UCI_OK) {
        syslog(LOG_ERR, "Failed to set option(NETSNMP-MOD-DYNAMIC set request)\n");
        goto PARAMETER_SET_ERR;
    }

    rc = uci_commit(c, &config.p, false);
    if (rc != UCI_OK) {
        syslog(LOG_ERR, "Failed to commit option(NETSNMP-MOD-DYNAMIC set request)\n");
        goto PARAMETER_SET_ERR;
    }

    uci_free_context(c);
    return rc;

    PARAMETER_SET_ERR:
    uci_free_context(c);
    return rc;
}