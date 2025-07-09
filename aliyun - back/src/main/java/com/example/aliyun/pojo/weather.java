package com.example.aliyun.pojo;

import com.google.gson.annotations.SerializedName;

import java.util.List;

// 定义Weather类来映射每天的天气数据
class Weather {
    String date;
    String text_day;
    String code_day;
    String text_night;
    String code_night;
    String high;
    String low;
    String precip;
    String wind_direction;
    String wind_direction_degree;
    String wind_speed;
    String wind_scale;
    String rainfall;
    String humidity;
}

// 定义Location类来映射地点数据
class Location {
    String id;
    String name;
    String country;
    String path;
    String timezone;
    @SerializedName("timezone_offset")
    String timezoneOffset;
}

// 定义Result类来映射结果数据
class Result {
    Location location;
    List<Weather> daily;
    String last_update;
}

// 定义Response类来映射整个响应数据
class Response1 {
    List<Result> results;
}