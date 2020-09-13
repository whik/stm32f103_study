#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "parsejson.h"
#include "malloc.h"
#include "usart2.h"
#include "my_lcd.h"
//#include "lcd_font.h"
//#include "ff.h"
//#include "exfuns.h"
//#include "delay.h"
#include "picture.h"
#include "rtc.h"
#include "weather.h"
#include "string.h"
#include "lcd_font.h"
#include "ILI93xx.h"

extern int SwitchToGbk(const unsigned char *pszBufIn, int nBufInLen, unsigned char *pszBufOut);


void parse_now_weatherr(void)
{
    cJSON *root;
    cJSON *results;
    cJSON *last_update;
    cJSON *loc_json, *daily_json;
    cJSON *forecast_json;
    int i = 0;
    char *loc_tmp, *weather_tmp, *update_tmp;
    char *str_tmp;
    char *GBK_str;

    root = mymalloc(sizeof(cJSON));
    results = mymalloc(sizeof(cJSON));
    last_update = mymalloc(sizeof(cJSON));
    loc_json = mymalloc(sizeof(cJSON));
    daily_json = mymalloc(sizeof(cJSON));
    forecast_json = mymalloc(sizeof(cJSON));

    loc_tmp = mymalloc(50);
    weather_tmp = mymalloc(50);
    update_tmp = mymalloc(50);
    str_tmp = mymalloc(50);
    GBK_str = mymalloc(50);

    memset(loc_tmp, 0, 50);
    memset(weather_tmp, 0, 50);
    memset(update_tmp, 0, 50);
    memset(str_tmp, 0, 50);
    memset(GBK_str, 0, 50);

    root = cJSON_Parse((const char *)USART2_RX_BUF);
//  root = cJSON_Parse((const char*)weather_str);
    if (root)
    {
        printf("开始解析\r\n");

        results = cJSON_GetObjectItem(root, "results");

        results = cJSON_GetArrayItem(results, 0);

        str_tmp = cJSON_Print(results);
        results = cJSON_Parse(str_tmp);      //loc_json字符串创建解析对象

        if (results)
        {
            loc_json = cJSON_GetObjectItem(results, "location");   //得到location键对应的值，是一个对象

            loc_tmp = cJSON_GetObjectItem(loc_json, "id") -> valuestring;
            printf("城市ID:%s\r\n", loc_tmp);
			
            loc_tmp = cJSON_GetObjectItem(loc_json, "name") -> valuestring;
            memset(GBK_str, 0, 50);
            SwitchToGbk((const u8 *)loc_tmp, strlen(loc_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
            printf("城市名称:%s\r\n", GBK_str);
			
            loc_tmp = cJSON_GetObjectItem(loc_json, "timezone") -> valuestring;
            printf("城市时区:%s\r\n", loc_tmp);

            daily_json = cJSON_GetObjectItem(results, "daily");
            if (daily_json)
            {
//                printf("daily json is:\n%s\r\n",cJSON_Print(daily_json));     //数组有三个元素，分别对应三天天气
                for (i = 0; i < 3; i++)
                {
                    forecast_json = cJSON_GetArrayItem(daily_json, i);
//                    printf("天气的json字符串:\n%s\r\n",cJSON_Print(forecast_json ));
                    weather_tmp = cJSON_GetObjectItem(forecast_json, "date") -> valuestring;
                    printf("日期:%s\r\n", weather_tmp);
                    weather_tmp = cJSON_GetObjectItem(forecast_json, "text_day") -> valuestring;
                    memset(GBK_str, 0, 50);
                    SwitchToGbk((const u8 *)weather_tmp, strlen(weather_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
                    printf("白天天气:%s\r\n", GBK_str);

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "text_night") -> valuestring;
                    memset(GBK_str, 0, 50);
                    SwitchToGbk((const u8 *)weather_tmp, strlen(weather_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
                    printf("晚上天气:%s\r\n", GBK_str);

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "high") -> valuestring;
                    printf("最高温度:%s 度\r\n", weather_tmp);

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "low") -> valuestring;
                    printf("最低温度:%s 度\r\n", weather_tmp);

                }
            }
            else
                printf("daily json格式错误\r\n");
            last_update = cJSON_GetObjectItem(results, "last_update");
            update_tmp = last_update->valuestring;
            if (last_update)
            {
                printf("最后更新时间:%s\r\n", update_tmp);
            }
            printf("天气数据解析成功\r\n\n");
        }
        else
        {
            printf("results格式错误:%s\r\n", cJSON_GetErrorPtr());
        }
    }
    else
    {
        printf("root格式错误\r\n");
    }

    cJSON_Delete(root);
    cJSON_Delete(results);
    cJSON_Delete(last_update);
    cJSON_Delete(loc_json);
    cJSON_Delete(daily_json);
    cJSON_Delete(forecast_json);

    myfree(root);
    myfree(results);
    myfree(last_update);
    myfree(loc_json);
    myfree(daily_json);
    myfree(forecast_json);

    myfree(loc_tmp);
    myfree(weather_tmp);
    myfree(update_tmp);
    myfree(str_tmp);
    myfree(GBK_str);
}

/*
void parse_seniverse_weatherr(void)
{
//  char weather_str[] =
//        "{\"results\":[{\"location\":{\"id\":\"WS10730EM8EV\",\"name\":\"深圳\",\"country\":\"CN\",\"path\":\"深圳,深圳,广东,中国\",\"timezone\":\"Asia/Shanghai\",\"timezone_offset\":\"+08:00\"},\"daily\":[{\"date\":\"2018-11-18\",\"text_day\":\"多云\",\"code_day\":\"4\",\"text_night\":\"多云\",\"code_night\":\"4\",\"high\":\"26\",\"low\":\"20\",\"precip\":\"\",\"wind_direction\":\"无持续风向\",\"wind_direction_degree\":\"\",\"wind_speed\":\"10\",\"wind_scale\":\"2\"},{\"date\":\"2018-11-19\",\"text_day\":\"小雨\",\"code_day\":\"13\",\"text_night\":\"小雨\",\"code_night\":\"13\",\"high\":\"25\",\"low\":\"20\",\"precip\":\"\",\"wind_direction\":\"无持续风向\",\"wind_direction_degree\":\"\",\"wind_speed\":\"10\",\"wind_scale\":\"2\"},{\"date\":\"2018-11-20\",\"text_day\":\"小雨\",\"code_day\":\"13\",\"text_night\":\"小雨\",\"code_night\":\"13\",\"high\":\"25\",\"low\":\"21\",\"precip\":\"\",\"wind_direction\":\"无持续风向\",\"wind_direction_degree\":\"\",\"wind_speed\":\"10\",\"wind_scale\":\"2\"}],\"last_update\":\"2018-11-18T11:00:00+08:00\"}]}";
//    cJSON *today_json, *tomorrow_json, *after_json;
    cJSON *root;
    cJSON *results;
    cJSON *last_update;
    cJSON *loc_json, *daily_json;
    cJSON *forecast_json;
    int i = 0;
    char *loc_tmp, *weather_tmp, *update_tmp;
    char *str_tmp;
    char *GBK_str;

    root = mymalloc(sizeof(cJSON));
    results = mymalloc(sizeof(cJSON));
    last_update = mymalloc(sizeof(cJSON));
    loc_json = mymalloc(sizeof(cJSON));
    daily_json = mymalloc(sizeof(cJSON));
    forecast_json = mymalloc(sizeof(cJSON));

    loc_tmp = mymalloc(50);
    weather_tmp = mymalloc(50);
    update_tmp = mymalloc(50);
    str_tmp = mymalloc(50);
    GBK_str = mymalloc(50);

    memset(loc_tmp, 0, 50);
    memset(weather_tmp, 0, 50);
    memset(update_tmp, 0, 50);
    memset(str_tmp, 0, 50);
    memset(GBK_str, 0, 50);

    root = cJSON_Parse((const char *)USART2_RX_BUF);
//  root = cJSON_Parse((const char*)weather_str);
    if (root)
    {
        printf("开始解析\r\n");

        results = cJSON_GetObjectItem(root, "results");

        results = cJSON_GetArrayItem(results, 0);

        str_tmp = cJSON_Print(results);
        results = cJSON_Parse(str_tmp);      //loc_json字符串创建解析对象

        if (results)
        {
            loc_json = cJSON_GetObjectItem(results, "location");   //得到location键对应的值，是一个对象

            loc_tmp = cJSON_GetObjectItem(loc_json, "id") -> valuestring;
            printf("城市ID:%s\r\n", loc_tmp);
            loc_tmp = cJSON_GetObjectItem(loc_json, "name") -> valuestring;
            memset(GBK_str, 0, 50);
            SwitchToGbk((const u8 *)loc_tmp, strlen(loc_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
            printf("城市名称:%s\r\n", GBK_str);
            loc_tmp = cJSON_GetObjectItem(loc_json, "timezone") -> valuestring;
            printf("城市时区:%s\r\n", loc_tmp);

            daily_json = cJSON_GetObjectItem(results, "daily");
            if (daily_json)
            {
//                printf("daily json is:\n%s\r\n",cJSON_Print(daily_json));     //数组有三个元素，分别对应三天天气
                for (i = 0; i < 3; i++)
                {
                    forecast_json = cJSON_GetArrayItem(daily_json, i);
//                    printf("天气的json字符串:\n%s\r\n",cJSON_Print(forecast_json ));
                    weather_tmp = cJSON_GetObjectItem(forecast_json, "date") -> valuestring;
                    printf("日期:%s\r\n", weather_tmp);
                    weather_tmp = cJSON_GetObjectItem(forecast_json, "text_day") -> valuestring;
                    memset(GBK_str, 0, 50);
                    SwitchToGbk((const u8 *)weather_tmp, strlen(weather_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
                    printf("白天天气:%s\r\n", GBK_str);

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "text_night") -> valuestring;
                    memset(GBK_str, 0, 50);
                    SwitchToGbk((const u8 *)weather_tmp, strlen(weather_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
                    printf("晚上天气:%s\r\n", GBK_str);

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "high") -> valuestring;
                    printf("最高温度:%s 度\r\n", weather_tmp);

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "low") -> valuestring;
                    printf("最低温度:%s 度\r\n", weather_tmp);

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "wind_direction") -> valuestring;
                    memset(GBK_str, 0, 50);
                    SwitchToGbk((const u8 *)weather_tmp, strlen(weather_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
                    printf("风向:%s 风\r\n", GBK_str);  //中文风向：西南

                    weather_tmp = cJSON_GetObjectItem(forecast_json, "wind_scale") -> valuestring;
                    printf("风力:%s\r\n\r\n", weather_tmp);
                }
            }
            else
                printf("daily json格式错误\r\n");
            last_update = cJSON_GetObjectItem(results, "last_update");
            update_tmp = last_update->valuestring;
            if (last_update)
            {
                printf("最后更新时间:%s\r\n", update_tmp);
            }
            printf("天气数据解析成功\r\n\n");
        }
        else
        {
            printf("results格式错误:%s\r\n", cJSON_GetErrorPtr());
        }
    }
    else
    {
        printf("root格式错误\r\n");
    }

    cJSON_Delete(root);
    cJSON_Delete(results);
    cJSON_Delete(last_update);
    cJSON_Delete(loc_json);
    cJSON_Delete(daily_json);
    cJSON_Delete(forecast_json);

    myfree(root);
    myfree(results);
    myfree(last_update);
    myfree(loc_json);
    myfree(daily_json);
    myfree(forecast_json);

    myfree(loc_tmp);
    myfree(weather_tmp);
    myfree(update_tmp);
    myfree(str_tmp);
    myfree(GBK_str);
}
*/
extern void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p);   //显示一个字符串,12/16字体
extern u16 POINT_COLOR;
u8 *WiFI = "TP-LINK_1F27EA";          //路由器SSID号

u8 parse_seniverse_weatherr(void)
{
//  char weather_str[] =
//        "{\"results\":[{\"location\":{\"id\":\"WS10730EM8EV\",\"name\":\"深圳\",\"country\":\"CN\",\"path\":\"深圳,深圳,广东,中国\",\"timezone\":\"Asia/Shanghai\",\"timezone_offset\":\"+08:00\"},\"daily\":[{\"date\":\"2018-11-18\",\"text_day\":\"多云\",\"code_day\":\"4\",\"text_night\":\"多云\",\"code_night\":\"4\",\"high\":\"26\",\"low\":\"20\",\"precip\":\"\",\"wind_direction\":\"无持续风向\",\"wind_direction_degree\":\"\",\"wind_speed\":\"10\",\"wind_scale\":\"2\"},{\"date\":\"2018-11-19\",\"text_day\":\"小雨\",\"code_day\":\"13\",\"text_night\":\"小雨\",\"code_night\":\"13\",\"high\":\"25\",\"low\":\"20\",\"precip\":\"\",\"wind_direction\":\"无持续风向\",\"wind_direction_degree\":\"\",\"wind_speed\":\"10\",\"wind_scale\":\"2\"},{\"date\":\"2018-11-20\",\"text_day\":\"小雨\",\"code_day\":\"13\",\"text_night\":\"小雨\",\"code_night\":\"13\",\"high\":\"25\",\"low\":\"21\",\"precip\":\"\",\"wind_direction\":\"无持续风向\",\"wind_direction_degree\":\"\",\"wind_speed\":\"10\",\"wind_scale\":\"2\"}],\"last_update\":\"2018-11-18T11:00:00+08:00\"}]}";
//    cJSON *today_json, *tomorrow_json, *after_json;
    cJSON *root;
    cJSON *results;
    cJSON *last_update;
    cJSON *loc_json, *daily_json;
    cJSON *forecast_json;
    int i = 0;
    char *loc_tmp, *weather_tmp, *update_tmp, *temperture_str;
    char *str_tmp;
    char *GBK_str;
	POINT_COLOR = WHITE;
//	Show_Str(200, 5, 200, 16, WiFI, 16, 0);		//显示WIFI信息

    root = mymalloc(100);		//解析不成功的情况
    results = mymalloc(sizeof(cJSON));
    last_update = mymalloc(sizeof(cJSON));
    loc_json = mymalloc(sizeof(cJSON));
    daily_json = mymalloc(sizeof(cJSON));
    forecast_json = mymalloc(sizeof(cJSON));

    loc_tmp = mymalloc(20);
    weather_tmp = mymalloc(10);
    update_tmp = mymalloc(10);
    str_tmp = mymalloc(10);
    GBK_str = mymalloc(10);

    memset(loc_tmp, 0, 10);
    memset(weather_tmp, 0, 10);
    memset(update_tmp, 0, 10);
    memset(str_tmp, 0, 10);
    memset(GBK_str, 0, 10);

    root = cJSON_Parse((const char *)USART2_RX_BUF);
//  root = cJSON_Parse((const char*)weather_str);
    if (root)
    {
        printf("开始解析\r\n");

        results = cJSON_GetObjectItem(root, "results");

        results = cJSON_GetArrayItem(results, 0);

        str_tmp = cJSON_Print(results);
        results = cJSON_Parse(str_tmp);      //loc_json字符串创建解析对象

        if (results)
        {
            loc_json = cJSON_GetObjectItem(results, "location");   //得到location键对应的值，是一个对象

            loc_tmp = cJSON_GetObjectItem(loc_json, "name") -> valuestring;

            SwitchToGbk((const u8 *)loc_tmp, strlen(loc_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
			POINT_COLOR = LBBLUE;
			Show_Str(5,10, 200, 16, GBK_str, 16, 0);		//显示城市名
			
            daily_json = cJSON_GetObjectItem(results, "daily");
            if (daily_json)
            {
//                printf("daily json is:\n%s\r\n",cJSON_Print(daily_json));     //数组有三个元素，分别对应三天天气
				LCD_Fill(0,170,320,200,BACK_COLOR);		//温度显示位置填充
				LCD_Fill(0,45,320,60,BACK_COLOR);		//日期显示位置填充
				LCD_Fill(210,220,320,240,BACK_COLOR);		//更新时间显示位置填充

				for (i = 0; i < 3; i++)
                {
                    forecast_json = cJSON_GetArrayItem(daily_json, i);
                    weather_tmp = cJSON_GetObjectItem(forecast_json, "date") -> valuestring;
                    printf("日期:%s\r\n", weather_tmp);
                    POINT_COLOR = GREEN;					
					show_date(i, weather_tmp);		//显示日期
					
					weather_tmp = cJSON_GetObjectItem(forecast_json, "text_day") -> valuestring;
                    memset(GBK_str, 0, 50);
                    SwitchToGbk((const u8 *)weather_tmp, strlen(weather_tmp), (u8 *)GBK_str);   //UTF-8转换为GBK
                    printf("白天天气:%s\r\n", GBK_str);

                    temperture_str = cJSON_GetObjectItem(forecast_json, "low") -> valuestring;
					strcat(temperture_str, "/");				
                    strcat(temperture_str, cJSON_GetObjectItem(forecast_json, "high") -> valuestring);
                    printf("温度:%s \r\n", temperture_str);
					POINT_COLOR = YELLOW;
					show_temperature(i,temperture_str);

					weather_tmp =  cJSON_GetObjectItem(forecast_json,"code_day") -> valuestring;												
					POINT_COLOR = LIGHTGREEN;
					show_weather_icon((u8 *)weather_tmp,i+1);
                }
            }
            else
			{
                printf("daily json格式错误\r\n");
				return 1;
			}
            strcpy(update_tmp, "更新时间:");
            last_update = cJSON_GetObjectItem(results, "last_update");
			strncat(update_tmp, (last_update->valuestring)+11,8);		//取原始数据中的一部分
            if (last_update)
            {
                printf("%s\r\n", update_tmp);
				POINT_COLOR = LBBLUE;
				Show_Str(210, 220, 200, 12, update_tmp, 12, 0);
            }
			else
			{
				printf("last_update解析失败\r\n");
				return 1;
			}
            printf("天气数据解析成功\r\n\n");
        }
        else
        {
            printf("results格式错误:%s\r\n", cJSON_GetErrorPtr());
			return 1;
		}
    }
    else
    {
        printf("root格式错误\r\n");
		return 1;
    }

    cJSON_Delete(root);
    cJSON_Delete(results);
    cJSON_Delete(last_update);
    cJSON_Delete(loc_json);
    cJSON_Delete(daily_json);
    cJSON_Delete(forecast_json);

    myfree(root);
    myfree(results);
    myfree(last_update);
    myfree(loc_json);
    myfree(daily_json);
    myfree(forecast_json);

    myfree(loc_tmp);
    myfree(weather_tmp);
    myfree(update_tmp);
    myfree(str_tmp);
    myfree(GBK_str);
	return 0;
}


void parse_bj_time(void)
{
    cJSON *root;
    cJSON *result_json;

    char *datetime_string, *week_string;
	char *str_tmp, *GBK_str;
    u8 year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

    root = mymalloc(10);
    result_json = mymalloc(10);

	str_tmp = mymalloc(10);
	GBK_str = mymalloc(5);
    datetime_string = mymalloc(10);
    week_string = mymalloc(5);

    memset(str_tmp, 0, 10);
    memset(GBK_str, 0, 5);
    memset(datetime_string, 0, 10);
    memset(week_string, 0, 5);

    root = cJSON_Parse((const char *)USART2_RX_BUF);
    if (root)
    {
        printf("开始解析数据\r\n");
        result_json =  cJSON_GetObjectItem(root, "result");  //获取result键对应的值

        if (result_json)
        {
            datetime_string = cJSON_GetObjectItem(result_json, "datetime_1")->valuestring;  //2019-01-20 15:03:00        //2是中文
			week_string = cJSON_GetObjectItem(result_json, "week_3")->valuestring;      //周日
            SwitchToGbk((const u8 *)week_string, strlen(week_string), (u8 *)GBK_str);   //UTF-8转换为GBK
			strcpy(str_tmp, GBK_str); 	//周日
			strncat(str_tmp, datetime_string+10, 11); 	//15:03:00
			POINT_COLOR = WHITE;
			Show_Str(210,10, 300, 16, str_tmp, 16, 0);	//显示星期和时间	
            year = (datetime_string[2] - '0') * 10 + (datetime_string[3] - '0');
            month = (datetime_string[5] - '0') * 10 + datetime_string[6] - '0';
            day = (datetime_string[8] - '0') * 10 + datetime_string[9] - '0';
            hour = (datetime_string[11] - '0') * 10 + datetime_string[12] - '0';
            minute = (datetime_string[14] - '0') * 10 + datetime_string[15] - '0';
            second = (datetime_string[17] - '0') * 10 + datetime_string[18] - '0';
			
            RTC_Set(2000 + year, month, day, hour, minute, second); //和北京时间同步

			printf("北京时间:%s \r\n", str_tmp);
            printf("解析成功\r\n");
        }
    }
	
    cJSON_Delete(result_json);
	cJSON_Delete(root);

    myfree(result_json);
    myfree(root);
    myfree(GBK_str);
    myfree(week_string);
    myfree(datetime_string);
    myfree(str_tmp);
}




