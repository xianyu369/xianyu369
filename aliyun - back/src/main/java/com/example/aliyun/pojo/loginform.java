package com.example.aliyun.pojo;

import lombok.Data;

@Data
public class loginform {
    public String username,password;
    public boolean rememberPwd,autoLogin;
}
