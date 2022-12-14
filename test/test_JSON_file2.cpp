/*
Copyright (C) 2019-2022 JingWeiZhangHuai <jingweizhanghuai@163.com>
Licensed under the Apache License, Version 2.0; you may not use this json except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/
//buuld: g++ -O2 test_JSON_file2.cpp -o test_JSON_file2.exe -lcjson -ljsoncpp -lyyjson -lmorn

#include "morn_util.h"

#include <fstream>
#include "jsoncpp/json/json.h"

#include "rapidjson/document.h"

#include "cjson/cJSON.h"

#include "nlohmann/json.hpp"

#include "yyjson.h"

#define fread(Data,Size,Num,Fl) mException(((int)fread(Data,Size,Num,Fl)!=Num),EXIT,"read file error")

int cjson_test1()
{
    FILE *f = fopen("./testdata/citm_catalog.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("cjson");
    cJSON * cjson_root = cJSON_Parse(jsondata); 
    int n=0;
    cJSON *performances_array = cJSON_GetObjectItem(cjson_root,"performances");
    for(int i=0;i<cJSON_GetArraySize(performances_array);i++)
    {
        cJSON *performances = cJSON_GetArrayItem(performances_array,i);
        cJSON *seatCategories_array = cJSON_GetObjectItem(performances,"seatCategories");
        for(int j=0;j<cJSON_GetArraySize(seatCategories_array);j++)
        {
            cJSON *seatCategories = cJSON_GetArrayItem(seatCategories_array,j);
            cJSON *areas_array = cJSON_GetObjectItem(seatCategories,"areas");
            for(int k=0;k<cJSON_GetArraySize(areas_array);k++)
            {
                cJSON *areas = cJSON_GetArrayItem(areas_array,k);
                cJSON *areaId = cJSON_GetObjectItem(areas,"areaId");
                int id=areaId->valueint;
                n++;
                // printf("id=%d\n",id);return 0;
            }
        }
    }
    mTimerEnd("cjson");

    free(jsondata);
    cJSON_Delete(cjson_root);
    return n;
}

int jsoncpp_test1()
{
    Json::Value root;
    Json::Reader reader;
    
    FILE *f = fopen("./testdata/citm_catalog.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);
    
    mTimerBegin("jsoncpp");
    reader.parse(jsondata,root);
    int n=0;
    Json::Value performances = root["performances"];
    for(int i=0;i<performances.size();i++)
    {
        Json::Value seatCategories = performances[i]["seatCategories"];
        for(int j=0;j<seatCategories.size();j++)
        {
            Json::Value areas = seatCategories[j]["areas"];
            for(int k=0;k<areas.size();k++)
            {
                Json::Value areaId = areas[k]["areaId"];
                int id = areaId.asInt();
                n++;
                // printf("id=%d\n",id);return 0;
            }
        }
    }
    mTimerEnd("jsoncpp");
    
    return n;
}

int nlohmann_test1()
{
    nlohmann::json nlohmannjson;
    
    FILE *f = fopen("./testdata/citm_catalog.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("nlohmann");
    nlohmannjson=nlohmann::json::parse(jsondata,jsondata+size);
    int n=0;
    nlohmann::json performances = nlohmannjson["performances"];
    for(int i=0;i<performances.size();i++)
    {
        nlohmann::json seatCategories=performances[i]["seatCategories"];
        for(int j=0;j<seatCategories.size();j++)
        {
            nlohmann::json areas=seatCategories[j]["areas"];
            for(int k=0;k<areas.size();k++)
            {
                int id = areas[k]["areaId"];
                n++;
                // printf("id=%d\n",id);
            }
        }
    }
    mTimerEnd("nlohmann");
    return n;
}

int rapidjson_test1()
{
    rapidjson::Document document;
    
    FILE *f = fopen("./testdata/citm_catalog.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("rapidjson");
    document.Parse(jsondata);
    int n=0;
    const rapidjson::Value& performances= document["performances"];
    for(int i=0;i<performances.Size();i++)
    {
        const rapidjson::Value& seatCategories=performances[i]["seatCategories"];
        for(int j=0;j<seatCategories.Size();j++)
        {
            const rapidjson::Value& areas=seatCategories[j]["areas"];
            for(int k=0;k<areas.Size();k++)
            {
                const rapidjson::Value& areaId=areas[k]["areaId"];
                int id = areaId.GetInt();
                n++;
                // printf("id=%d\n",id);
            }
        }
    }
    mTimerEnd("rapidjson");

    free(jsondata);
    return n;
}

int yyjson_test1()
{
    FILE *f = fopen("./testdata/citm_catalog.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("yyjson");
    yyjson_doc *doc = yyjson_read(jsondata,size,0);
    int n=0;
    yyjson_val *root=yyjson_doc_get_root(doc);
    yyjson_val *performances_array=yyjson_obj_get(root,"performances");
    for(int i=0;i<yyjson_arr_size(performances_array);i++)
    {
        yyjson_val *performances = yyjson_arr_get(performances_array,i);
        yyjson_val *seatCategories_array = yyjson_obj_get(performances,"seatCategories");
        for(int j=0;j<yyjson_arr_size(seatCategories_array);j++)
        {
            yyjson_val *seatCategories = yyjson_arr_get(seatCategories_array,j);
            yyjson_val *areas_array = yyjson_obj_get(seatCategories,"areas");
            for(int k=0;k<yyjson_arr_size(areas_array);k++)
            {
                yyjson_val *areas = yyjson_arr_get(areas_array,k);
                yyjson_val *areaId = yyjson_obj_get(areas,"areaId");
                int id=yyjson_get_uint(areaId);
                n++;
                // printf("id=%d\n",id);
            }
        }
    }
    mTimerEnd("yyjson");
    
    yyjson_doc_free(doc);
    free(jsondata);
    return n;
}

int Morn_test1()
{
    MObject *mornjson=mObjectCreate();
    
    FILE *f = fopen("./testdata/citm_catalog.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);
    
    mTimerBegin("Morn Json");
    mornjson->string=jsondata;mornjson->size=size;
    struct JSONNode *json = mJSONLoad(mornjson);
    int n=0;
    struct JSONNode *performances_array = mJSONRead(json,"performances");
    for(int i=0;i<performances_array->num;i++)
    {
        struct JSONNode *performances = mJSONRead(performances_array,i);
        struct JSONNode *seatCategories_array = mJSONRead(performances,"seatCategories");
        for(int j=0;j<seatCategories_array->num;j++)
        {
            struct JSONNode *seatCategories = mJSONRead(seatCategories_array,j);
            struct JSONNode *areas_array = mJSONRead(seatCategories,"areas");
            for(int k=0;k<areas_array->num;k++)
            {
                struct JSONNode *areas = mJSONRead(areas_array,k);
                struct JSONNode *areaId=mJSONRead(areas,"areaId");
                int id=areaId->dataS32;
                n++;
                // printf("id=%d\n",id);
            }
        }
    }
    mTimerEnd("Morn Json");

    mObjectRelease(mornjson);
    return n;
}

void test1()
{
    printf("\n");
    int n;

    n=rapidjson_test1();
    printf("get %d areaId\n\n",n);

    n=cjson_test1();
    printf("get %d areaId\n\n",n);

    n=jsoncpp_test1();
    printf("get %d areaId\n\n",n);
    
    n=nlohmann_test1();
    printf("get %d areaId\n\n",n);
    
    n=yyjson_test1();
    printf("get %d areaId\n\n",n);

    n=Morn_test1();
    printf("get %d areaId\n\n",n);
}

int cjson_test2()
{
    FILE *f = fopen("./testdata/canada.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("cJSON");
    cJSON *root = cJSON_Parse(jsondata); 
    int n=0;
    cJSON *features_array = cJSON_GetObjectItem(root,"features");
    cJSON *features       = cJSON_GetArrayItem(features_array,0);
    cJSON *geometry       = cJSON_GetObjectItem(features,"geometry");
    cJSON *coordinates0   = cJSON_GetObjectItem(geometry,"coordinates");
    for(int i=0;i<cJSON_GetArraySize(coordinates0);i++)
    {
        cJSON *coordinates1 = cJSON_GetArrayItem(coordinates0,i);
        for(int j=0;j<cJSON_GetArraySize(coordinates1);j++)
        {
            cJSON *coordinates2 = cJSON_GetArrayItem(coordinates1,j);
            double x=cJSON_GetArrayItem(coordinates2,0)->valuedouble;
            double y=cJSON_GetArrayItem(coordinates2,1)->valuedouble;
            n++;
            // printf("x=%f,y=%f\n",x,y);
        }
    }
    mTimerEnd("cJSON");

    free(jsondata);
    cJSON_Delete(root);
    return n;
}

int jsoncpp_test2()
{
    Json::Value root;
    Json::Reader reader;
    
    FILE *f = fopen("./testdata/canada.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);
    
    mTimerBegin("jsoncpp");
    reader.parse(jsondata,root);
    int n=0;
    Json::Value coordinates0 = root["features"][0]["geometry"]["coordinates"];
    for(int j=0;j<coordinates0.size();j++)
    {
        Json::Value coordinates1 = coordinates0[j];
        for(int i=0;i<coordinates1.size();i++)
        {
            Json::Value coordinates2 = coordinates1[i];
            double x=coordinates2[0].asDouble();
            double y=coordinates2[1].asDouble();
            n++;
            // printf("x=%f,y=%f\n",x,y);
        }
    }
    mTimerEnd("jsoncpp");
    
    return n;
}

int nlohmann_test2()
{
    nlohmann::json nlohmannjson;
    
    FILE *f = fopen("./testdata/canada.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("nlohmann");
    nlohmannjson=nlohmann::json::parse(jsondata,jsondata+size);
    int n=0;
    nlohmann::json coordinates0 = nlohmannjson["features"][0]["geometry"]["coordinates"];
    for(int j=0;j<coordinates0.size();j++)
    {
        nlohmann::json coordinates1=coordinates0[j];
        for(int i=0;i<coordinates1.size();i++)
        {
            nlohmann::json coordinates2=coordinates1[i];
            double x=coordinates2[0];
            double y=coordinates2[1];
            n++;
            // printf("x=%f,y=%f\n",x,y);
        }
    }
    mTimerEnd("nlohmann");
    
    return n;
}

int rapidjson_test2()
{
    rapidjson::Document doc;
    
    FILE *f = fopen("./testdata/canada.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("rapidjson");
    doc.Parse(jsondata);
    int n=0;
    const rapidjson::Value& coordinates0= doc["features"][0]["geometry"]["coordinates"];
    for(int j=0;j<coordinates0.Size();j++)
    {
        const rapidjson::Value& coordinates1= coordinates0[j];
        for(int i=0;i<coordinates1.Size();i++)
        {
            const rapidjson::Value& coordinates2= coordinates1[i];
            double x= coordinates2[0].GetDouble();
            double y= coordinates2[1].GetDouble();
            n++;
            // printf("x=%f,y=%f\n",x,y);
        }
    }
    mTimerEnd("rapidjson");

    free(jsondata);
    return n;
}

int yyjson_test2()
{
    FILE *f = fopen("./testdata/canada.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);

    mTimerBegin("yyjson");
    yyjson_doc *doc = yyjson_read(jsondata,size,0); 
    int n=0;
    yyjson_val *root           = yyjson_doc_get_root(doc);
    yyjson_val *features_array = yyjson_obj_get(root,"features");
    yyjson_val *features       = yyjson_arr_get(features_array,0);
    yyjson_val *geometry       = yyjson_obj_get(features,"geometry");
    yyjson_val *coordinates0   = yyjson_obj_get(geometry,"coordinates");
    for(int i=0;i<yyjson_arr_size(coordinates0);i++)
    {
        yyjson_val *coordinates1 = yyjson_arr_get(coordinates0,i);
        for(int j=0;j<yyjson_arr_size(coordinates1);j++)
        {
            yyjson_val *coordinates2 = yyjson_arr_get(coordinates1,j);
            double x=yyjson_get_real(yyjson_arr_get(coordinates2,0));
            double y=yyjson_get_real(yyjson_arr_get(coordinates2,1));
            n++;
            // printf("x=%f,y=%f\n",x,y);
        }
    }
    mTimerEnd("yyjson");

    free(jsondata);
    yyjson_doc_free(doc);
    return n;
}

int Morn_test2()
{
    MObject *mornjson=mObjectCreate();
    
    FILE *f = fopen("./testdata/canada.json","rb");
    int size = fsize(f);
    char *jsondata=(char *)malloc(size);
    fread(jsondata,size,1,f);
    fclose(f);
    
    mTimerBegin("Morn json");
    mornjson->string=jsondata;mornjson->size=size;
    struct JSONNode *json=mJSONLoad(mornjson);
    int n=0;
    struct JSONNode *coordinates0=mJSONRead(json,"features[0].geometry.coordinates");
    for (int j=0;j<coordinates0->num;j++)
    {
        struct JSONNode *coordinates1 = mJSONRead(coordinates0,j);
        for (int i=0;i<coordinates1->num;i++)
        {
            struct JSONNode *coordinates2 = mJSONRead(coordinates1,i);
            double x=mJSONRead(coordinates2,0)->dataD64;
            double y=mJSONRead(coordinates2,1)->dataD64;
            n++;
            // printf("x=%f,y=%f\n",x,y);
        }
    }
    mTimerEnd("Morn json");
    
    mObjectRelease(mornjson);
    return n;
}

void test2()
{
    printf("\n");
    int n;

    n=rapidjson_test2();
    printf("get %d coordinates\n\n",n);

    n=cjson_test2();
    printf("get %d coordinates\n\n",n);

    n=jsoncpp_test2();
    printf("get %d coordinates\n\n",n);

    n=nlohmann_test2();
    printf("get %d coordinates\n\n",n);
    
    n=yyjson_test2();
    printf("get %d coordinates\n\n",n);

    n=Morn_test2();
    printf("get %d coordinates\n\n",n);
}


#define TEST_TIME 100

void cjson_test3(const char *filename)
{
    printf("\nfor %s:\n",filename);
    MString *jsondata=mObjectCreate();
    mFile(jsondata,filename);

    mTimerBegin("rapidjson");
    for(int i=0;i<TEST_TIME;i++)
    {
        cJSON *root = cJSON_Parse(jsondata->string);
        cJSON_Delete(root);
    }
    mTimerEnd("rapidjson");
    mObjectRelease(jsondata);
}

void rapidjson_test3(const char *filename)
{
    printf("\nfor %s:\n",filename);
    MString *jsondata=mObjectCreate();
    mFile(jsondata,filename);

    mTimerBegin("rapidjson");
    for(int i=0;i<TEST_TIME;i++)
    {
        rapidjson::Document doc;
        doc.Parse(jsondata->string);
    }
    mTimerEnd("rapidjson");
    mObjectRelease(jsondata);
}

void nlohmann_test3(const char *filename)
{
    printf("\nfor %s:\n",filename);
    MString *jsondata=mObjectCreate();
    mFile(jsondata,filename);

    mTimerBegin("nlohmann");
    for(int i=0;i<TEST_TIME;i++)
    {
        nlohmann::json nlohmannjson;
        nlohmannjson=nlohmann::json::parse(jsondata->string,jsondata->string+jsondata->size);
    }
    mTimerEnd("nlohmann");
    mObjectRelease(jsondata);
}

void jsoncpp_test3(const char *filename)
{
    printf("\nfor %s:\n",filename);
    MString *jsondata=mObjectCreate();
    mFile(jsondata,filename);

    mTimerBegin("jsoncpp");
    for(int i=0;i<TEST_TIME;i++)
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse(jsondata->string,root);
    }
    mTimerEnd("jsoncpp");
    mObjectRelease(jsondata);
}

void yyjson_test3(const char *filename)
{
    printf("\nfor %s:\n",filename);
    MString *jsondata=mObjectCreate();
    mFile(jsondata,filename);

    mTimerBegin("yyjson");
    for(int i=0;i<TEST_TIME;i++)
    {
        yyjson_doc *doc = yyjson_read(jsondata->string,jsondata->size,0);
        yyjson_doc_get_root(doc);
        yyjson_doc_free(doc);
    }
    mTimerEnd("yyjson");
    mObjectRelease(jsondata);
}

void Morn_test3(const char *filename)
{
    printf("\nfor %s:\n",filename);
    MString *jsondata=mObjectCreate();
    mFile(jsondata,filename);

    mTimerBegin("Morn json");
    for(int i=0;i<TEST_TIME;i++)
        mJSONLoad(jsondata);
    mTimerEnd("Morn json");
    mObjectRelease(jsondata);
}

void test_cjson()
{
    cjson_test3("./testdata/canada.json");
    cjson_test3("./testdata/citm_catalog.json");
    cjson_test3("./testdata/twitter.json");
    cjson_test3("./testdata/github_events.json");
    cjson_test3("./testdata/apache_builds.json");
    cjson_test3("./testdata/mesh.json");
    cjson_test3("./testdata/mesh.pretty.json");
    cjson_test3("./testdata/update-center.json");
}

void test_rapidjson()
{
    rapidjson_test3("./testdata/canada.json");
    rapidjson_test3("./testdata/citm_catalog.json");
    rapidjson_test3("./testdata/twitter.json");
    rapidjson_test3("./testdata/github_events.json");
    rapidjson_test3("./testdata/apache_builds.json");
    rapidjson_test3("./testdata/mesh.json");
    rapidjson_test3("./testdata/mesh.pretty.json");
    rapidjson_test3("./testdata/update-center.json");
}

void test_nlohmann()
{
    nlohmann_test3("./testdata/canada.json");
    nlohmann_test3("./testdata/citm_catalog.json");
    nlohmann_test3("./testdata/twitter.json");
    nlohmann_test3("./testdata/github_events.json");
    nlohmann_test3("./testdata/apache_builds.json");
    nlohmann_test3("./testdata/mesh.json");
    nlohmann_test3("./testdata/mesh.pretty.json");
    nlohmann_test3("./testdata/update-center.json");
}

void test_jsoncpp()
{
    jsoncpp_test3("./testdata/canada.json");
    jsoncpp_test3("./testdata/citm_catalog.json");
    jsoncpp_test3("./testdata/twitter.json");
    jsoncpp_test3("./testdata/github_events.json");
    jsoncpp_test3("./testdata/apache_builds.json");
    jsoncpp_test3("./testdata/mesh.json");
    jsoncpp_test3("./testdata/mesh.pretty.json");
    jsoncpp_test3("./testdata/update-center.json");
}

void test_yyjson()
{
    yyjson_test3("./testdata/canada.json");
    yyjson_test3("./testdata/citm_catalog.json");
    yyjson_test3("./testdata/twitter.json");
    yyjson_test3("./testdata/github_events.json");
    yyjson_test3("./testdata/apache_builds.json");
    yyjson_test3("./testdata/mesh.json");
    yyjson_test3("./testdata/mesh.pretty.json");
    yyjson_test3("./testdata/update-center.json");
}

void test_Morn()
{
    Morn_test3("./testdata/canada.json");
    Morn_test3("./testdata/citm_catalog.json");
    Morn_test3("./testdata/twitter.json");
    Morn_test3("./testdata/github_events.json");
    Morn_test3("./testdata/apache_builds.json");
    Morn_test3("./testdata/mesh.json");
    Morn_test3("./testdata/mesh.pretty.json");
    Morn_test3("./testdata/update-center.json");
}

int main(int argc,char *argv[])
{
    if(argc!=2) {printf("try as: \"test_json_file2.exe test1\" or \"test_json_file2.exe test2\"\n"); return 0;}
    else if(strcmp(argv[1],"test1"    )==0) test1();
    else if(strcmp(argv[1],"test2"    )==0) test2();
    else if(strcmp(argv[1],"cjson"    )==0) test_cjson();
    else if(strcmp(argv[1],"rapidjson")==0) test_rapidjson();
    else if(strcmp(argv[1],"nlohmann" )==0) test_nlohmann();
    else if(strcmp(argv[1],"jsoncpp"  )==0) test_jsoncpp();
    else if(strcmp(argv[1],"yyjson"   )==0) test_yyjson();
    else if(strcmp(argv[1],"Morn"     )==0) test_Morn();
    return 0;
}
