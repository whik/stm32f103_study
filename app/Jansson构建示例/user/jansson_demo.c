#include "jansson_demo.h"

/*
//普通键值对JSON对象
{
	"status": true,
	"age": 20,
	"score": 78.4,
	"name": "whik1194",
	"blog": "https://blog.csdn.net/whik1194",
	"wechat": "mcu149"
}
*/
uint8_t Jansson1_Demo(void)
{
    json_t *root;
    json_t *blog_string;
    char *str;
    
    root = json_object();

    json_object_set_new(root, "status", json_boolean(JSON_TRUE));
    json_object_set_new(root, "age", json_integer(20));
    json_object_set_new(root, "score", json_real(78.4));
    json_object_set_new(root, "name", json_string("whik1194"));
    
    blog_string = json_string("https://blog.csdn.net/whik1194");
    json_object_set_new(root, "blog", blog_string);
    
    json_object_set_new(root, "wechat", json_string("mcu149"));
    
    str = json_dumps(root, JSON_PRESERVE_ORDER);
    
    json_decref(root);
    
    LOG1("\r\njson_string:\r\n%s\r\n", str);
    return 0;
}   

/*
//嵌套的JSON对象
{
	"success": "1",
	"result": {
		"timestamp": "1592640249",
		"datetime_1": "2020-06-20 16:04:09",
		"week_1": 6,
		"week_2": "Saturday"
	}
}
*/
uint8_t Jansson2_Demo(void)
{
    json_t *root;
    json_t *result_obj;
    char *str;

    root = json_object();
    result_obj = json_object();

    json_object_set_new(result_obj, "timestamp", json_string("1592640249"));
    json_object_set_new(result_obj, "datetime_1", json_string("2020-06-20 16:04:09"));
    json_object_set_new(result_obj, "week_1", json_integer(1));
    json_object_set_new(result_obj, "week_2", json_string("Saturday"));
    
    json_object_set_new(root, "success", json_string("1"));
    json_object_set_new(root, "result", result_obj);
    
    str = json_dumps(root, JSON_PRESERVE_ORDER);
    json_decref(root);

    LOG1("\r\njson_string:\r\n%s\r\n", str);
    return 0;
}

/*
//包含数组的JSON对象构建
{
	"location": [{
		"name": "Faye",
		"address": "Beijing"
	}, {
		"name": "Andy",
		"address": "ShangHai"
	}, {
		"name": "Lisa",
		"address": "ShenZhen"
	}],
	"time": "2018-11-17"
}
*/
uint8_t Jansson3_Demo(void)
{
    json_t *root;
    json_t *array_obj;
    char *str;
    
    char name[3][20] = {"Faye", "Andy", "Lisa"};
    char address[3][20] = {"Beijing", "ShangHai", "ShenZhen"};
    uint8_t idx = 0;
    json_t *array_value;
    
    root = json_object();
    
    array_obj = json_array();
    array_value = json_object();
    
    for(idx = 0; idx < 3; idx++)
    {
        json_object_set_new(array_value, name[idx], json_string(address[idx]));
        json_array_insert_new(array_obj, idx, array_value);
//        json_array_append_new(array_obj, array_value);
    }
        
    json_object_set_new(root, "location", array_obj);       //location键的值是一个数组
    json_object_set_new(root, "time", json_string("2018-11-17"));
    
    str = json_dumps(root, JSON_PRESERVE_ORDER);
    json_decref(root);

    LOG1("\r\njson_string:\r\n%s\r\n", str);
    return 0;
}

