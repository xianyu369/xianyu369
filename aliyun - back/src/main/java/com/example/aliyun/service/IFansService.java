package com.example.aliyun.service;

import com.example.aliyun.entity.Fans;
import com.baomidou.mybatisplus.extension.service.IService;

import java.util.List;

/**
 * <p>
 *  服务类
 * </p>
 *
 * @author xianyu
 * @since 2025-06-26
 */
public interface IFansService extends IService<Fans> {
    public List<String> getfan();
}
