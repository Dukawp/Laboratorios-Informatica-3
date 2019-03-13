#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "../include/auxiliares.h"
#include "../include/common.h"

User parserUsers(xmlDocPtr doc,xmlNodePtr cur);
Post parserPosts(xmlDocPtr doc,xmlNodePtr cur);
Tag parserTags(xmlDocPtr doc,xmlNodePtr cur);
TAD_community parsing(TAD_community com, char* dump_path);