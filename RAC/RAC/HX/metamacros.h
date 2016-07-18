//
//  metamacros.h
//  RAC
//
//  Created by 旭旭 on 16/7/17.
//  Copyright © 2016年 yd. All rights reserved.
//

#ifndef metamacros_h
#define metamacros_h


//可变参数传入一个个表达式，会执行这些个表达式，然后返回true
//如可以这样传入
//void f1(){
//    printf("dddd\n");
//}
//BOOL ret = metamacro_exprify(f1(),[self method1]);
#define metamacro_exprify(...) ((__VA_ARGS__), true)



/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.................以下是实现细节..........................*/

#define metamacro_stringify_(VALUE) # VALUE
//#define ssss(VALUE) @(metamacro_stringify_(VALUE))
#define metamacro_concat_(A, B) A ## B



#endif /* metamacros_h */
