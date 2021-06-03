#include "common.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
//refer http://blog.chinaunix.net/uid-25695950-id-3921541.html

void xml_test() {
    //create Dom tree
    doc=xmlParseFile(argv[1]);
    if(doc==NULL)
    {
     printf("ERROR: Loading xml file failed.\n");
     exit(1);
    }

    // get root node
    cur=xmlDocGetRootElement(doc);
    if(cur==NULL)
    {
     printf("ERROR: empty file\n");
     xmlFreeDoc(doc);
     exit(2);
    }

    //walk the tree
    cur=cur->xmlChildrenNode; //get sub node
    while(cur !=NULL)
    {
     name=(char*)(cur->name);
     value=(char*)xmlNodeGetContent(cur);
     printf("DEBUG: name is: %s, value is: %s\n", name, value);
     xmlFree(value);
     xmlChar* szAttr = xmlGetProp(cur,BAD_CAST "attribute");
     printf("DEBUG: attribute is: %s\n", (char *)szAttr );
     xmlFree(szAttr);
     cur=cur->next;
    }

    // release resource of xml parser in libxml2
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}
