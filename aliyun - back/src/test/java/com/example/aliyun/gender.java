package com.example.aliyun;

import com.baomidou.mybatisplus.generator.FastAutoGenerator;
import com.baomidou.mybatisplus.generator.engine.FreemarkerTemplateEngine;

import java.nio.file.Paths;

public class gender {
    public static void main(String[] args) {
        FastAutoGenerator.create("jdbc:dm://localhost:5236/ALIYUN", "SYSDBA", "021910wenD")
                .globalConfig(builder -> builder
                        .author("xianyu")
                        .outputDir(Paths.get(System.getProperty("user.dir")) + "/src/main/java")
//                        .commentDate("yyyy-MM-dd")
                )
                .packageConfig(builder -> builder
                        .parent("com.example.aliyun")
                        .entity("entity")
                        .mapper("mapper")
                        .service("service")
                        .serviceImpl("service.impl")
                        .xml("mapper.xml")
                )
                .strategyConfig(builder -> builder
                        .entityBuilder()
                        .enableLombok()
                )
                .templateEngine(new FreemarkerTemplateEngine())
                .execute();
    }
}
