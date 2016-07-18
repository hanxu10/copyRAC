//
//  metamacros.h
//  RAC
//
//  Created by 旭旭 on 16/7/17.
//  Copyright © 2016年 yd. All rights reserved.
//

/*.......................................................*/
/*...*//*       可以声明可变参数的宏，         
                例如：#define eprintf(...) fprintf (stderr, __VA_ARGS__)                                                        *//*...*/
/*...*//*       当这个宏被调用的时候，它的参数列表中的最后一个有名参数的后面的所有参数，包括逗号，变为可变参数。这些可变参数去替换__VA_ARGS__     *//*...*/
/*...*//*       eprintf ("%s:%d: ", input_file, lineno)
                        ==>  fprintf (stderr, "%s:%d: ", input_file, lineno)                                                    *//*...*/
/*...*//*       如果你的宏很复杂，你可能想要使用一个有意义的名字而不是使用__VA_ARGS__。CPP允许这样，作为一个扩展。你可以在...之前加上一个       *//*...*/
/*...*//*       参数的名字，这个名字用来代替可变参数，这样的话上面的宏可以这样写：                                                        *//*...*/
/*...*//*            #define eprintf(args...) fprintf (stderr, args)  
                使用这个扩展的话，就不能再同时使用__VA_ARGS__了。                  
                
                你还可以有有名的参数，例如可以定义上面宏的如下：
                     #define eprintf(format, ...) fprintf (stderr, format, __VA_ARGS__)
                这种定义看上去描述性很强，但不幸的是它很不健壮：因为你必须在format后面提供至少一个参数。在标准C中，你不能省略分割有名参数的逗
                号，于是，如果你的可变参数啥都不传入的话，你将获得一个语法错误，因为在format后面多了一个逗号。
                eprintf("success!\n", );
                        ==> fprintf(stderr, "success!\n", );
                
                GNU CPP有两个扩展来处理这个问题。第一个，你可以在后面留一个逗号，eprintf ("success!\n") ==> fprintf(stderr, "success!\n", );
                第二个，把##用于逗号和可变参数之间时，##有特殊的含义。如果你这样定义宏   #define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__) 
                ，当你的可变参数啥都不传入的时候，##前面的逗号会被删除。
        *//*...*/
/*.......................................................*/
/*.......................................................................................................................................*/



/*.......................................................*/
/*...*//*                           #的使用                                                   *//*...*/
/*...*//*             #的功能是将其后面的宏参数字符串化，简单地说就是宏变量的其左右加上双引号             *//*...*/
/*...*//*                                                                           *//*...*/
/*...*//*                                                                           *//*...*/
/*...*//*                                                                              *//*...*/
/*...*//*
/*.......................................................*/
/*.......................................................*/




#ifndef metamacros_h
#define metamacros_h


//可变参数传入一个个表达式，会执行这些个表达式，然后返回true
//如可以这样传入
//void f1(){
//    printf("dddd\n");
//}
//BOOL ret = metamacro_exprify(f1(),[self method1]);
#define metamacro_exprify(...) ((__VA_ARGS__), true)



//返回value的字符串表示
//int ss = 10;
//char *s = metamacro_stringify(ss);
//s等于"ss"
#define metamacro_stringify(VALUE) metamacro_stringify_(VALUE)



//int ss = 10;
//int s = metamacro_concat(s, s);
//相当于int s = ss;
//s等于10
#define metamacro_concat(A, B) metamacro_concat_(A, B)


//返回第N（从0开始）个可变参数。至少得给N+1个参数。N在 0 和 20 之间，包括0和20。
#define metamacro_at(N,...) metamacro_concat(metamacro_at, N)(__VA_ARGS__)


//返回可变参数的个数。
//很巧妙啊
//第20个就是可变参数的个数，如果传入2个可变参数，则总共有22个，第20（从0开始）个就是2.
#define metamacro_argcount(...) metamacro_at(20, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)



//#define ppp(y,x) NSLog(@"haha-%d-%@",y,x)
//
//@implementation ViewController
//
//- (void)viewDidLoad {
//    [super viewDidLoad];
//    
//    metamacro_foreach(ppp, ;, @"2",@"3");
//    NSLog(@"");
//}
//打印结果：
//2016-07-18 23:08:51.434 StudyRAC[10912:604443] haha-0-2
//2016-07-18 23:08:51.434 StudyRAC[10912:604443] haha-1-3
//这个宏MACRO接受两个参数。SEP设置为;。然后传入可变参数。
//与metamacro_foreach_cxt一样，除了不需要提供CONTEXT参数。只有index和当前参数被传入MARCO。
//可以参考http://paul-samuels.com/blog/2014/03/21/how-does-it-work-meta-macros/
#define metamacro_foreach(MACRO, SEP, ...) metamacro_foreach_cxt(metamacro_foreach_iter, SEP, MACRO, __VA_ARGS__)

#define metamacro_foreach_cxt(MACRO, SEP, CONTEXT, ...) metamacro_concat(metamacro_foreach_cxt, metamacro_argcount(__VA_ARGS__))(MACRO, SEP, CONTEXT, __VA_ARGS__)


/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.................以下是实现细节..........................*/

#define metamacro_stringify_(VALUE) # VALUE

#define metamacro_concat_(A, B) A ## B


//传入一个宏的名字MACRO，这个宏有两个参数，分别为INDEX和ARG
#define metamacro_foreach_iter(INDEX, MACRO, ARG) MACRO(INDEX, ARG)


//int ss = 10;
//int sss = 11;
//int ssss = metamacro_head(sss,sssssss);
//虽然sssssss没有被定义，但是不会出错。因为宏替换之后就sssssss就没了，相当于int ssss = sss;
#define metamacro_head(First, ...) First

#define metamacro_tail(First, ...) __VA_ARGS__




#define metamacro_at0(...) metamacro_head(__VA_ARGS__)
#define metamacro_at1(_0, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at2(_0, _1, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at3(_0, _1, _2, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at4(_0, _1, _2, _3, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at5(_0, _1, _2, _3, _4, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at6(_0, _1, _2, _3, _4, _5, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at7(_0, _1, _2, _3, _4, _5, _6, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at8(_0, _1, _2, _3, _4, _5, _6, _7, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at9(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at17(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at18(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at19(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) metamacro_head(__VA_ARGS__)
#define metamacro_at20(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) metamacro_head(__VA_ARGS__)


/*---------------------------------------------------------------------------------------------------------*/

#define metamacro_foreach_cxt0(MACRO, SEP, CONTEXT)

#define metamacro_foreach_cxt1(MACRO, SEP, CONTEXT, _0) MACRO(0, CONTEXT, _0)

#define metamacro_foreach_cxt2(MACRO, SEP, CONTEXT, _0, _1) \
    metamacro_foreach_cxt1(MACRO, SEP, CONTEXT, _0) \
    SEP \
    MACRO(1, CONTEXT, _1)

#define metamacro_foreach_cxt3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    metamacro_foreach_cxt2(MACRO, SEP, CONTEXT, _0, _1) \
    SEP \
    MACRO(2, CONTEXT, _2)

#define metamacro_foreach_cxt4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    metamacro_foreach_cxt3(MACRO, SEP, CONTEXT, _0, _1, _2) \
    SEP \
    MACRO(3, CONTEXT, _3)

#define metamacro_foreach_cxt5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    metamacro_foreach_cxt4(MACRO, SEP, CONTEXT, _0, _1, _2, _3) \
    SEP \
    MACRO(4, CONTEXT, _4)

#define metamacro_foreach_cxt6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    metamacro_foreach_cxt5(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4) \
    SEP \
    MACRO(5, CONTEXT, _5)

#define metamacro_foreach_cxt7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    metamacro_foreach_cxt6(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5) \
    SEP \
    MACRO(6, CONTEXT, _6)

#define metamacro_foreach_cxt8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    metamacro_foreach_cxt7(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6) \
    SEP \
    MACRO(7, CONTEXT, _7)

#define metamacro_foreach_cxt9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    metamacro_foreach_cxt8(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7) \
    SEP \
    MACRO(8, CONTEXT, _8)

#define metamacro_foreach_cxt10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    metamacro_foreach_cxt9(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8) \
    SEP \
    MACRO(9, CONTEXT, _9)

#define metamacro_foreach_cxt11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    metamacro_foreach_cxt10(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    SEP \
    MACRO(10, CONTEXT, _10)

#define metamacro_foreach_cxt12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    metamacro_foreach_cxt11(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    SEP \
    MACRO(11, CONTEXT, _11)

#define metamacro_foreach_cxt13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    metamacro_foreach_cxt12(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    SEP \
    MACRO(12, CONTEXT, _12)

#define metamacro_foreach_cxt14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    metamacro_foreach_cxt13(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) \
    SEP \
    MACRO(13, CONTEXT, _13)

#define metamacro_foreach_cxt15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    metamacro_foreach_cxt14(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) \
    SEP \
    MACRO(14, CONTEXT, _14)

#define metamacro_foreach_cxt16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    metamacro_foreach_cxt15(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) \
    SEP \
    MACRO(15, CONTEXT, _15)

#define metamacro_foreach_cxt17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    metamacro_foreach_cxt16(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) \
    SEP \
    MACRO(16, CONTEXT, _16)

#define metamacro_foreach_cxt18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    metamacro_foreach_cxt17(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) \
    SEP \
    MACRO(17, CONTEXT, _17)

#define metamacro_foreach_cxt19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    metamacro_foreach_cxt18(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17) \
    SEP \
    MACRO(18, CONTEXT, _18)

#define metamacro_foreach_cxt20(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19) \
    metamacro_foreach_cxt19(MACRO, SEP, CONTEXT, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18) \
    SEP \
    MACRO(19, CONTEXT, _19)

/*---------------------------------------------------------------------------------------------------------*/








#endif /* metamacros_h */
