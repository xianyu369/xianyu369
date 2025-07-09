package com.example.aliyun.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.example.aliyun.entity.Fans;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

import java.util.List;


/**
 * <p>
 *  Mapper 接口
 * </p>
 *
 * @author xianyu
 * @since 2025-06-26
 */
@Mapper
public interface FansMapper extends BaseMapper<Fans> {
    @Select("select fan from ALIYUN.FANS")
    public List<String> getfan();
}
