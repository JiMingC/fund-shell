#include <stdio.h>
#include "common.h"
#include <curl/curl.h>
#include <string.h>

fundInfo_s fundInfo[30];
char data_buf[1024];
//输出到字符串再打印到屏幕上
ssize_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	strcat(stream, (char *)ptr);
	puts(stream);
//    memcpy(data_buf, (char*)stream, size*nmemb);
	return size*nmemb;
}
 
//输出到文件
/*
ssize_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}
*/

char *res_buf = NULL;
int shift;
size_t copy_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    int res_size;

    res_size = size * nmemb;
    res_buf = realloc(res_buf, shift+res_size + 1);
    memcpy(res_buf + shift, ptr, res_size);
    shift += res_size;
    return size * nmemb;
}

int fundGetDataByCode(CURL *curl, char* str, char *buf) {
    char tmp_str[2048] = {0};
    sprintf(tmp_str, "http://fund.eastmoney.com/pingzhongdata/%s.js?v=20160518155842", str);
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, tmp_str);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);//-k 
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);//-k
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//数据请求到以后的回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);//选择输出到字符串
        curl_easy_perform(curl);//这里是执行请求
        curl_easy_cleanup(curl);
        memcpy(buf, data_buf, sizeof(data_buf));
        return sizeof(buf);
    }
    return 0;
}

int curlDataToJson(char* src_buf, char* json_out) {
    char *ptr = src_buf;
    char *end = NULL;
    ptr += strlen("jsonpgz")+1;
    if (*ptr == ')')
        return 0;
    end = ptr;
    int num = 0;
    while(*end != ';') {
        end++;
        num++;
    }
    num--;
    memset(json_out, 0, num);
    memcpy(json_out, ptr, num);
    return num;
}

int fundGetObjFromBuf(char* curl_data, char* obj_str, char *obj_val, short Opt) {
    int num = 0;
    if (curl_data) {
        char *ptr = NULL;
        char *end = NULL;
        ptr = strstr(curl_data, obj_str);
        if (ptr) {
            ptr += strlen(obj_str)+3;
            if (Opt == 1)
                ptr++;
            end = ptr;
            char c = *ptr;
            unsigned char a = 1;
            while (*end != ';') {
                end++;
                num++;
            }
        }

        num = Opt ? num-1 : num;
        memset(obj_val, 0, num);
        memcpy(obj_val, ptr, num);
        //printf("end:%s\n",end);
    }
    return num;
}

void fundGetInfobyKey (char *str) {
    char *obj = (char*)malloc(1000);
    memset(obj, 0, 1000);
    fundGetObjFromBuf(res_buf, str, obj, 1);
    printf("%s\t", obj);
    free(obj);
}

void fundPriTittle(void) {
    printf("Num\tCode\tname\t\t\t\tl_val\tc_val\tgain\tupdate\n");
}

#if 0
void fundGetInfo(char *buf, int len) {
    char * buf_obj = malloc(shift);
    memcpy(buf_obj, buf, len);
    fundPriTittle();
    fundGetInfobyKey("fS_code");
    fundGetInfobyKey("fS_name");
    cJSON *js;
    js = JsonParse_objectInArray(buf_obj, "y");
    printf("%.3f\t", js->valuedouble);
    js = JsonParse_objectInArray(buf_obj, "equityReturn");
    printf("%.3f\t", js->valuedouble);
    printf("\n");
    free(buf_obj);
}
#endif

int fundGetCurlDate(CURL *curl, char* curl_addr) {
    if (!curl)
        return 1;
    static char str[20480];
    curl_easy_setopt(curl, CURLOPT_URL, curl_addr);//这是请求的url
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);//-k 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);//-k
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, copy_data);//数据请求到以后的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);//选择输出到字符串
    curl_easy_perform(curl);//这里是执行请求
//    curl_easy_cleanup(curl);
    return 0;
}

static int count = 1;
void fundGetInfoByCode(CURL *curl, char* code) {
    cJSON *js;
    char curl_addr[2048] = {0};
    sprintf(curl_addr, "http://fundgz.1234567.com.cn/js/%s.js?rt=1463558676006", code);
    fundGetCurlDate(curl, curl_addr);
    char * src_js = malloc(shift);
    int num = curlDataToJson(res_buf, src_js);
    memset(res_buf, 0, shift);
    shift = 0;
    if (!num) {
        LOGD("%s curl data abnormal\n", code);
        free(src_js);
        return;
    }
    printf("%3d\t", count);
    count++;
    js = JsonParse_object(src_js, "fundcode");
    printf("%s\t", js->valuestring);
    js = JsonParse_object(src_js, "name");
    printf("%-35s\t", js->valuestring);
    js = JsonParse_object(src_js, "dwjz");
    printf("%s\t", js->valuestring);
    js = JsonParse_object(src_js, "gsz");
    printf("%s\t", js->valuestring);
    js = JsonParse_object(src_js, "gszzl");
    if(js->valuestring[0] != '-')
        printf("+");
    printf("%s%%\t", js->valuestring);
    js = JsonParse_object(src_js, "gztime");
    printf("%s\t", js->valuestring+5);
    free(src_js);
    //printf("\n");
}

char* code[30] = {
    "009791",
    "011613",
    "260108",
    "007349",
    "001475",
    "008888",
    "270042",
    "010755",
    "502010",
    "010125",
    "005495",
    "008086",
    "161725",
    "160221",
    "164403",
    "100038",
    "010198",
    "501058",
    "005491",
    "502023",
    "006328",
    "501005"
    "end",
};

#define CODE_NUM 30
void fundGetInfo(CURL *curl) {
    fundPriTittle();
    int i = 0;
    for(i = 0; i < CODE_NUM; i++) {
        if (strlen(code[i]) != 6)
            return;
        fundGetInfoByCode(curl, code[i]);
    }
}

void fundGetInfoFromXml(fundInfo_s *a, CURL *curl, int num) {
    fundPriTittle();
    int i = 0;
    for(i = 0; i < num; i++) {
        //LOGD("%d %s\n", i, (a+i)->f_code);
        if(strlen((a+i)->f_code) != 6)
            continue;
        fundGetInfoByCode(curl, (a+i)->f_code);
        printf("%s\n", (a+i)->status);
    }
}

void fundInfopri(fundInfo_s *a) {
    int i;
    for(i = 0; i < 30; i++) {
        LOGD("%d:%s\n", i, (a+i)->f_code);
    }
}

int main(int argc, char *argv[])
{
    CURL *curl2;
	CURLcode res2;
	//FILE *fp2;
	//struct curl_slist *list=NULL;
	//list = curl_slist_append(list, argv[1]);//这个其实是-H但是这边没用到所以注释
	//list = curl_slist_append(list, argv[2]);//有几个-H头就append几次
	static char str[20480];
	res2 = curl_global_init(CURL_GLOBAL_ALL);
	curl2 = curl_easy_init();
    //xml_test();
    //fundInfo = calloc(30, sizeof(fundInfo));
    int f_num = xmlLoadInfo(fundInfo);
    LOGD("%d\n", f_num);
    fundInfopri(fundInfo);
    fundGetInfoFromXml(fundInfo, curl2, f_num);
    //fundGetInfo(curl2);
	curl_global_cleanup();
#if 0
	if(curl2) 
	{
		//fp2=fopen("UsefullInfo.json", "w+");
		//curl_easy_setopt(curl2, CURLOPT_URL, "http://fund.eastmoney.com/pingzhongdata/001186.js?v=20160518155842");//这是请求的url
		curl_easy_setopt(curl2, CURLOPT_URL, "http://fundgz.1234567.com.cn/js/001186.js?rt=1463558676006");//这是请求的url
		//curl_easy_setopt(curl2, CURLOPT_POSTFIELDS, "username=root@pam&password=nicaiba_88");//这是post的内容
		//curl_easy_setopt(curl2, CURLOPT_HTTPHEADER, list);//若需要带头，则写进list
		curl_easy_setopt(curl2, CURLOPT_SSL_VERIFYPEER, 0);//-k
		curl_easy_setopt(curl2, CURLOPT_SSL_VERIFYHOST, 0);//-k
		//curl_easy_setopt(curl2, CURLOPT_VERBOSE, 1);//这是请求过程的调试log
		curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, copy_data);//数据请求到以后的回调函数
		curl_easy_setopt(curl2, CURLOPT_WRITEDATA, str);//选择输出到字符串
		//curl_easy_setopt(curl2, CURLOPT_WRITEDATA, fp2);//选择输出到文件
		res2 = curl_easy_perform(curl2);//这里是执行请求
		curl_easy_cleanup(curl2);
		//fclose(fp2);
	}
#endif
    //sql3_test();
	return 0;
}
