#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"cJSON.h"

cJSON* JsonParse_objectInArray(char* buf, char* name)
{
    //从缓冲区中解析出JSON结构
    cJSON *json = cJSON_Parse(buf);
    cJSON *node = NULL;
    //cJOSN_GetObjectItem 根据key来查找json节点 若果有返回非空
    node = cJSON_GetObjectItem(json, name);
    if(node == NULL)
    {
        printf("%s node == NULL\n", name);
    }
    else
    {
        printf("found %s node\n", name);
    }
    /*
    //判断是否有key是string的项 如果有返回1 否则返回0
    if(1 == cJSON_HasObjectItem(json, name))
    {
        printf("found %s node\n", name);
    }
    else
    {
        printf("not found %s node\n", name);
    }
    */
    //非array类型的node 被当做array获取size的大小是未定义的行为 不要使用
    cJSON *tnode = NULL;
    
    node = cJSON_Parse(buf);
    int size = cJSON_GetArraySize(node);
    printf("%d\n", size);
    cJSON *sub_json, *js_name;
    char *p = NULL;
//    int i;
//    for(i=0;i<size;i++)
//    {
        tnode = cJSON_GetArrayItem(node, size - 1);
        switch(tnode->type) {
            case cJSON_Object:
                p = cJSON_PrintUnformatted(tnode);
                sub_json = cJSON_Parse(p);
                if (sub_json) {
                    js_name = cJSON_GetObjectItem(sub_json, name);
                    printf("%s is %lf\n", name, js_name->valuedouble);
                }
                break;
        }
//    }
    return js_name;
}

int JsonParseValue(char* buf, char* obj_name)
{
    //从缓冲区中解析出JSON结构
    printf("%s\n", buf);
    cJSON *json = cJSON_Parse(buf);
    cJSON *node = NULL;
    //cJOSN_GetObjectItem 根据key来查找json节点 若果有返回非空
    node = cJSON_GetObjectItem(json, obj_name);
    if(node == NULL)
    {
        printf("%s node == NULL\n", obj_name);
    }
    else
    {
        printf("found %s node\n", obj_name);
    }
    //判断是否有key是string的项 如果有返回1 否则返回0
    if(1 == cJSON_HasObjectItem(json, obj_name))
    {
        printf("found %s node\n", obj_name);
    }
    else
    {
        printf("not found %s node\n", obj_name);
    }
/*
    node = cJSON_GetObjectItem(json, obj_name);
    if(node->type == cJSON_Array)
    {
        printf("array size is %d",cJSON_GetArraySize(node));
    }
*/
    //非array类型的node 被当做array获取size的大小是未定义的行为 不要使用
    cJSON *tnode = NULL;
    
    node = cJSON_Parse(buf);
    int size = cJSON_GetArraySize(node);
    printf("%d\n", size);
    int i;
    for(i=0;i<size;i++)
    {
        tnode = cJSON_GetArrayItem(node,i);
        if(tnode->type == cJSON_String)
        {
            printf("value[%d]:%s\n",i,tnode->valuestring);
        }
        else
        {
            printf("node' type is not string\n");
        }
    }
/*
    cJSON_ArrayForEach(tnode,node)
    {
        if(tnode->type == cJSON_String)
        {
            printf("int forEach: vale:%s\n",tnode->valuestring);
        }
        else
        {
            printf("node's type is not string\n");
        }
    }
*/
    return 0;
}


