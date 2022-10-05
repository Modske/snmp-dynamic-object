#define CFG_PATH         "/etc/config"
#define CFG_NAME         "snmp_module_testing"
#define CFG_OPTION       "name"
#define CFG_SECTION      "author"
#define CFG_NAME_SECTION "snmp_module_testing.author"
#define BUFF_SIZE        64

int uci_parameter_set(char *data);
int uci_parameter_get(char *buff);