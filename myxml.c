#include "common.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
//refer http://blog.chinaunix.net/uid-25695950-id-3921541.html

#define XML_FILE "test.xml"

void fundInfoSet(fundInfo_s *fundInfo, char *name, char* value) {
    if (strstr(name,"name"))
        strcpy(fundInfo->f_name, value);
    if (strstr(name,"code"))
        strcpy(fundInfo->f_code, value);

    return;
}

int xmlLoadInfo(fundInfo_s *a) {
    int num = xmlGetNodeNum();
    LOGD("%d\n",num);
    //a = (fundInfo_s*)calloc(num, sizeof(fundInfo_s));

    xmlDocPtr doc=NULL;
    xmlNodePtr cur=NULL;
    xmlNodePtr f_node = NULL;
    char* name=NULL;
    char* value=NULL;

    xmlKeepBlanksDefault (0);

    //create Dom tree
    doc=xmlParseFile(XML_FILE);
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
    num = 0;
    while(cur !=NULL)
    {
        f_node =  cur->xmlChildrenNode;
        while(f_node != NULL) {
            name=(char*)(f_node->name);
            value=(char*)xmlNodeGetContent(f_node);
            fundInfoSet(a+num, name, value);
            //LOGD("DEBUG: name is: %s, value is: %s\n", name, value);
            xmlFree(value);
            f_node = f_node->next;
        }
        LOGD("%s\t%s\n", (a+num)->f_name, (a+num)->f_code);
        num++;
        cur = cur->next;
    }

    // release resource of xml parser in libxml2
    xmlFreeDoc(doc);
//    xmlCleanupParser();

    LOGD("finish %d\n", num);
    return num;
}

int xmlGetNodeNum() {
    int num = 0;
    xmlDocPtr doc=NULL;
    xmlNodePtr cur=NULL;
    xmlNodePtr f_node = NULL;
    char* name=NULL;
    char* value=NULL;

    xmlKeepBlanksDefault (0);

    //create Dom tree
    doc=xmlParseFile(XML_FILE);
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
        num++;
        cur = cur->next;
    }

    // release resource of xml parser in libxml2
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return num;
}

int xml_test() {
    int num = 0;
    xmlDocPtr doc=NULL;
    xmlNodePtr cur=NULL;
    xmlNodePtr f_node = NULL;
    char* name=NULL;
    char* value=NULL;

    xmlKeepBlanksDefault (0);

    //create Dom tree
    doc=xmlParseFile(XML_FILE);
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
        f_node =  cur->xmlChildrenNode;
        while(f_node != NULL) {
            name=(char*)(f_node->name);
            value=(char*)xmlNodeGetContent(f_node);
            printf("DEBUG: name is: %s, value is: %s\n", name, value);
            xmlFree(value);
            xmlChar* szAttr = xmlGetProp(cur,BAD_CAST "attribute");
            printf("DEBUG: attribute is: %s\n", (char *)szAttr );
            xmlFree(szAttr);
            f_node = f_node->next;
        }
        num++;
        cur = cur->next;
    }

    // release resource of xml parser in libxml2
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return num;
}
