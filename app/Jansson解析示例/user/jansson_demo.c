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
uint8_t Jansson1_Demo(uint8_t *text)
{
    //解析串口接收的数据
//    const char *text = "{\"status\":true,\"age\":20,\"score\":78.4,\"name\":\"whik1194\",\"blog\":\"https://blog.csdn.net/whik1194\",\"wechat\":\"mcu149\"}";
    json_error_t error;
    json_t *root;
    json_t *status;
    char *name, *blog, *wechat;
    int age;
    double score;
    
    root = json_loads((const char*)text, 0, &error); 
    if(json_is_object(root))
    {
        LOG1("root format correct\r\n");
        
        /* string 类型的解析 */
        name = (char *)json_string_value(json_object_get(root, "name"));
        blog = (char *)json_string_value(json_object_get(root, "blog"));
        wechat = (char *)json_string_value(json_object_get(root, "wechat"));
        
        LOG1("name: %s\r\n", name);
        LOG1("blog: %s\r\n", blog);
        LOG1("wechat: %s\r\n", wechat);
        
        /* int 类型的解析 */
        age = json_integer_value(json_object_get(root, "age"));
        LOG1("age: %d\r\n", age);
        
        /* double 类型的解析 */
        score = json_real_value(json_object_get(root, "score"));
        LOG1("score: %.1f\r\n", score);
        
        /* bool 类型的解析 */
        status = json_object_get(root, "status");
        if(json_is_true(status))
            LOG1("status: true\r\n");
        else if(json_is_true(status))
            LOG1("status: true\r\n");
    }
    else
    {
        LOG1("root format error:%d-%s\r\n", error.line, error.text);
        return 1;
    }
    return 0;
}   

/*
//JSON对象的嵌套
{
	"success": "1",
	"result": {
		"timestamp": "1592640249",
		"datetime_1": "2020-06-20 16:04:09",
		"week_1": "6",
		"week_2": "Saturday"
	}
}
*/
uint8_t Jansson2_Demo(void)
{
    const char *text = "{\"success\":\"1\",\"result\":{\"timestamp\":\"1592640249\",\"datetime_1\":\"2020-06-20 16:04:09\",\"week_1\":\"6\",\"week_2\":\"Saturday\"}}";
    json_error_t error;
    json_t *root, *result;
    
    char *success, *timestamp, *datetime_1, *week_1;

    root = json_loads((const char*)text, 0, &error); 
    if(json_is_object(root))
    {
        LOG1("root format correct\r\n");
        success = (char *)json_string_value(json_object_get(root, "success"));
        LOG1("success:%s\r\n", success);
        
        result = json_object_get(root, "result");
        if(json_is_object(result))
        {
            timestamp = (char *)json_string_value(json_object_get(result, "timestamp"));
            datetime_1 = (char *)json_string_value(json_object_get(result, "datetime_1"));
            week_1 = (char *)json_string_value(json_object_get(result, "week_1"));
            
            LOG1("timestamp:%s\r\n", timestamp);
            LOG1("datetime_1:%s\r\n", datetime_1);
            LOG1("week_1:%s\r\n", week_1);
        }
        else 
        {
            LOG1("reuslt format error\r\n");
            return 1;
        }
    }
    else
    {
        LOG1("root format error:%d-%s\r\n", error.line, error.text);
        return 1;
    }
    return 0;
}

/*
//包含数组的JSON对象
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
    const char *text = "{\"location\":[{\"name\":\"Faye\",\"address\":\"Beijing\"},{\"name\":\"Andy\",\"address\":\"ShangHai\"},{\"name\":\"Lisa\",\"address\":\"ShenZhen\"}],\"time\":\"2018-11-17\"}";
   
    json_error_t error;
    json_t *root;
    json_t *location_arr;
    json_t *location;
    
    char *name, *address;
    char *time;
    int idx;
    
    root = json_loads((const char*)text, 0, &error); 
    if(json_is_object(root))
    {
        LOG1("root format correct\r\n");
        time = (char *)json_string_value(json_object_get(root, "time"));
        location_arr = json_object_get(root, "location");
        
        if(json_is_array(location_arr))
        {
            for(idx = 0; idx < json_array_size(location_arr); idx++)
            {
                location = json_array_get(location_arr, idx);
                name = (char *)json_string_value(json_object_get(location, "name"));
                address = (char *)json_string_value(json_object_get(location, "address"));
                LOG1("%d: %s-%s\r\n", idx, name, address);
            }
        }
    }
    else
    {
        LOG1("root format error:%d-%s\r\n", error.line, error.text);
        return 1;
    }
    return 0;
}

