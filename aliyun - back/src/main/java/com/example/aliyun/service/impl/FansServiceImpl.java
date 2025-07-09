package com.example.aliyun.service.impl;

import com.example.aliyun.entity.Fans;
import com.example.aliyun.mapper.FansMapper;
import com.example.aliyun.service.IFansService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * <p>
 *  服务实现类
 * </p>
 *
 * @author xianyu
 * @since 2025-06-26
 */
@Service
public class FansServiceImpl extends ServiceImpl<FansMapper, Fans> implements IFansService {
    @Autowired
    FansMapper fansMapper;
    public List<String> getfan() {
        return fansMapper.getfan();
    }
}
