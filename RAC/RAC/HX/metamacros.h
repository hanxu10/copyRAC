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
/*...*//*             #的功能是将其后面的宏参数字符串化，简单地说就是宏变量的其左右加上双引号            *//*...*/
/*...*//*                                                                                    *//*...*/
/*...*//*                                                                                    *//*...*/
/*...*//*                                                                                    *//*...*/
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



//和上面的metamacro_foreach_cxt一样。
#define metamacro_foreach_cxt_recursive(MACRO, SEP, CONTEXT, ...) metamacro_concat(metamacro_foreach_cxt_recursive, metamacro_argcount(__VA_ARGS__))(MACRO, SEP, CONTEXT, __VA_ARGS__)


//对比之前的
//#define metamacro_foreach(MACRO, SEP, ...) metamacro_foreach_cxt(metamacro_foreach_iter, SEP, MACRO, __VA_ARGS__)
//之前的这个宏是把（0，参数0），（1，参数1）等等挨个传给MACRO，然后用SEP分割；
//这个宏是把BASE和参数0拼接起来，BASE和参数1拼接起来，等等。
//两个宏的主要区别是第一个传入metamacro_foreach_iter        ，它的作用是#define metamacro_foreach_iter(INDEX, MACRO, ARG) MACRO(INDEX, ARG)
//第二个传入               metamacro_foreach_concat_iter ，它的作用是#define metamacro_foreach_concat_iter(INDEX, BASE, ARG) metamacro_foreach_concat_iter_(BASE, ARG)，
//加了一个INDEX参数纯属为了把它变为3个参数的宏。
//举个例子：
//#define op1 NSLog(@"op11")
//#define op2 NSLog(@"op22")
//#define op3 NSLog(@"op33")
//- (void)viewDidLoad {
//    [super viewDidLoad];
//    metamacro_foreach_concat(op, ;, 1,2,3);
//}
//输出的结果为：
//2016-07-19 19:39:47.769 StudyProgram[48614:12983758] op11
//2016-07-19 19:39:47.769 StudyProgram[48614:12983758] op22
//2016-07-19 19:39:47.769 StudyProgram[48614:12983758] op33
#define metamacro_foreach_concat(BASE, SEP, ...) metamacro_foreach_cxt(metamacro_foreach_concat_iter, SEP, BASE, __VA_ARGS__)


//重复COUNT次，每次使用index（从0开始）和CONTEXT调用MACRO
#define metamacro_for_cxt(COUNT, MACRO, SEP, CONTEXT) metamacro_concat(metamacro_for_cxt, COUNT)(MACRO, SEP, CONTEXT)








//返回第一个参数，至少需要传入一个参数
#define metamacro_head(...) metamacro_head_(__VA_ARGS__, 0)

//返回除了第一个参数以外的所有参数，至少需要传入两个参数
#define metamacro_tail(...) metamacro_tail_(__VA_ARGS__)


//返回前N（N最多到20）个可变参数。至少提供N个可变参数
#define metamacro_take(N, ...) metamacro_concat(metamacro_take, N)(__VA_ARGS__)


//移除前N（N最多到20）个可变参数。至少提供N个可变参数
#define metamacro_drop(N, ...) metamacro_concat(metamacro_drop, N)(__VA_ARGS__)


//把VAL减1，VAL必须是大于等于0，小于等于20
#define metamacro_dec(VAL) metamacro_at(VAL, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19)

//把VAL加1，VAL必须是大于等于0，小于等于20
#define metamacro_inc(VAL) metamacro_at(VAL, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21)

//对B取反，B只能取0或1
#define metamacro_not(B) metamacro_at(B, 1, 0)

//举个例子：
//int s = 1;
//metamacro_if_eq(0, 0) (int a = 111;) (int b =2;);
//s = a;
//此时int a = 111;被执行了，s = 111；而b没有定义；
//
//再举个例子：
//metamacro_if_eq(1, 1) (NSLog(@"相等")) (NSLog(@"不相等"));
//此时会打印“相等”
//
//    metamacro_if_eq(A, B) (M)(N)          如果A等于B，就保留M,否则保留N
//
//  举例演示展开过程：
//  metamacro_if_eq(1,1) 展开为 metamacro_if_eq1(1)
//  接着展开为metamacro_if_eq0(metamacro_dec(1))
//  接着展开为metamacro_if_eq0(0)
//  接着展开为metamacro_if_eq0_0
//  metamacro_if_eq0_0(...)这个宏的定义为：#define metamacro_if_eq0_0(...) __VA_ARGS__ metamacro_consume_
//  给metamacro_if_eq0_0这个宏接上(M) (N),就变为metamacro_if_eq0_0(M) (N) 接着展开为M metamacro_consume_(N)
//  最终就变为了M。
#define metamacro_if_eq(A, B) metamacro_concat(metamacro_if_eq, A)(B)

//与metamacro_if_eq相同。当metamacro_if_eq由于嵌套调用失败的时候使用；
//感觉除了名字不一样，其他的完全一样，应该是嵌套使用metamacro_if_eq会有问题，所以搞一个名字不一样的。
//
#define metamacro_if_eq_recursive(A, B) metamacro_concat(metamacro_if_eq_recursive, A)(B)

//如果N（0到20）是奇数就返回0，是偶数就返回1.0认为是偶数。
#define metamacro_is_even(N) metamacro_at(N, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1)

/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
/*.......................................................*/
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
#define metamacro_head_(First, ...) First

#define metamacro_tail_(First, ...) __VA_ARGS__


#define metamacro_consume_(...)

#define metamacro_expand_(...) __VA_ARGS__


//重新实现，这样metamacro_concat()不会嵌套
#define metamacro_foreach_concat_iter(INDEX, BASE, ARG) metamacro_foreach_concat_iter_(BASE, ARG)
#define metamacro_foreach_concat_iter_(BASE, ARG) BASE ## ARG

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
//注意MACRO有三个参数
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

//先取一个头部，再从tail里取几个
#define metamacro_take0(...)
#define metamacro_take1(...) metamacro_head(__VA_ARGS__)
#define metamacro_take2(...) metamacro_head(__VA_ARGS__), metamacro_take1(metamacro_tail(__VA_ARGS__))
#define metamacro_take3(...) metamacro_head(__VA_ARGS__), metamacro_take2(metamacro_tail(__VA_ARGS__))
#define metamacro_take4(...) metamacro_head(__VA_ARGS__), metamacro_take3(metamacro_tail(__VA_ARGS__))
#define metamacro_take5(...) metamacro_head(__VA_ARGS__), metamacro_take4(metamacro_tail(__VA_ARGS__))
#define metamacro_take6(...) metamacro_head(__VA_ARGS__), metamacro_take5(metamacro_tail(__VA_ARGS__))
#define metamacro_take7(...) metamacro_head(__VA_ARGS__), metamacro_take6(metamacro_tail(__VA_ARGS__))
#define metamacro_take8(...) metamacro_head(__VA_ARGS__), metamacro_take7(metamacro_tail(__VA_ARGS__))
#define metamacro_take9(...) metamacro_head(__VA_ARGS__), metamacro_take8(metamacro_tail(__VA_ARGS__))
#define metamacro_take10(...) metamacro_head(__VA_ARGS__), metamacro_take9(metamacro_tail(__VA_ARGS__))
#define metamacro_take11(...) metamacro_head(__VA_ARGS__), metamacro_take10(metamacro_tail(__VA_ARGS__))
#define metamacro_take12(...) metamacro_head(__VA_ARGS__), metamacro_take11(metamacro_tail(__VA_ARGS__))
#define metamacro_take13(...) metamacro_head(__VA_ARGS__), metamacro_take12(metamacro_tail(__VA_ARGS__))
#define metamacro_take14(...) metamacro_head(__VA_ARGS__), metamacro_take13(metamacro_tail(__VA_ARGS__))
#define metamacro_take15(...) metamacro_head(__VA_ARGS__), metamacro_take14(metamacro_tail(__VA_ARGS__))
#define metamacro_take16(...) metamacro_head(__VA_ARGS__), metamacro_take15(metamacro_tail(__VA_ARGS__))
#define metamacro_take17(...) metamacro_head(__VA_ARGS__), metamacro_take16(metamacro_tail(__VA_ARGS__))
#define metamacro_take18(...) metamacro_head(__VA_ARGS__), metamacro_take17(metamacro_tail(__VA_ARGS__))
#define metamacro_take19(...) metamacro_head(__VA_ARGS__), metamacro_take18(metamacro_tail(__VA_ARGS__))
#define metamacro_take20(...) metamacro_head(__VA_ARGS__), metamacro_take19(metamacro_tail(__VA_ARGS__))

/*---------------------------------------------------------------------------------------------------------*/

//移除1个，就是取tail
//移除2个，就是取取tail后再取tail
#define metamacro_drop0(...) __VA_ARGS__
#define metamacro_drop1(...) metamacro_tail(__VA_ARGS__)
#define metamacro_drop2(...) metamacro_drop1(metamacro_tail(__VA_ARGS__))
#define metamacro_drop3(...) metamacro_drop2(metamacro_tail(__VA_ARGS__))
#define metamacro_drop4(...) metamacro_drop3(metamacro_tail(__VA_ARGS__))
#define metamacro_drop5(...) metamacro_drop4(metamacro_tail(__VA_ARGS__))
#define metamacro_drop6(...) metamacro_drop5(metamacro_tail(__VA_ARGS__))
#define metamacro_drop7(...) metamacro_drop6(metamacro_tail(__VA_ARGS__))
#define metamacro_drop8(...) metamacro_drop7(metamacro_tail(__VA_ARGS__))
#define metamacro_drop9(...) metamacro_drop8(metamacro_tail(__VA_ARGS__))
#define metamacro_drop10(...) metamacro_drop9(metamacro_tail(__VA_ARGS__))
#define metamacro_drop11(...) metamacro_drop10(metamacro_tail(__VA_ARGS__))
#define metamacro_drop12(...) metamacro_drop11(metamacro_tail(__VA_ARGS__))
#define metamacro_drop13(...) metamacro_drop12(metamacro_tail(__VA_ARGS__))
#define metamacro_drop14(...) metamacro_drop13(metamacro_tail(__VA_ARGS__))
#define metamacro_drop15(...) metamacro_drop14(metamacro_tail(__VA_ARGS__))
#define metamacro_drop16(...) metamacro_drop15(metamacro_tail(__VA_ARGS__))
#define metamacro_drop17(...) metamacro_drop16(metamacro_tail(__VA_ARGS__))
#define metamacro_drop18(...) metamacro_drop17(metamacro_tail(__VA_ARGS__))
#define metamacro_drop19(...) metamacro_drop18(metamacro_tail(__VA_ARGS__))
#define metamacro_drop20(...) metamacro_drop19(metamacro_tail(__VA_ARGS__))


/*---------------------------------------------------------------------------------------------------------*/

#define metamacro_if_eq0(VALUE) metamacro_concat(metamacro_if_eq0_, VALUE)

#define metamacro_if_eq0_0(...) __VA_ARGS__ metamacro_consume_
#define metamacro_if_eq0_1(...) metamacro_expand_
#define metamacro_if_eq0_2(...) metamacro_expand_
#define metamacro_if_eq0_3(...) metamacro_expand_
#define metamacro_if_eq0_4(...) metamacro_expand_
#define metamacro_if_eq0_5(...) metamacro_expand_
#define metamacro_if_eq0_6(...) metamacro_expand_
#define metamacro_if_eq0_7(...) metamacro_expand_
#define metamacro_if_eq0_8(...) metamacro_expand_
#define metamacro_if_eq0_9(...) metamacro_expand_
#define metamacro_if_eq0_10(...) metamacro_expand_
#define metamacro_if_eq0_11(...) metamacro_expand_
#define metamacro_if_eq0_12(...) metamacro_expand_
#define metamacro_if_eq0_13(...) metamacro_expand_
#define metamacro_if_eq0_14(...) metamacro_expand_
#define metamacro_if_eq0_15(...) metamacro_expand_
#define metamacro_if_eq0_16(...) metamacro_expand_
#define metamacro_if_eq0_17(...) metamacro_expand_
#define metamacro_if_eq0_18(...) metamacro_expand_
#define metamacro_if_eq0_19(...) metamacro_expand_
#define metamacro_if_eq0_20(...) metamacro_expand_

#define metamacro_if_eq1(VALUE) metamacro_if_eq0(metamacro_dec(VALUE))
#define metamacro_if_eq2(VALUE) metamacro_if_eq1(metamacro_dec(VALUE))
#define metamacro_if_eq3(VALUE) metamacro_if_eq2(metamacro_dec(VALUE))
#define metamacro_if_eq4(VALUE) metamacro_if_eq3(metamacro_dec(VALUE))
#define metamacro_if_eq5(VALUE) metamacro_if_eq4(metamacro_dec(VALUE))
#define metamacro_if_eq6(VALUE) metamacro_if_eq5(metamacro_dec(VALUE))
#define metamacro_if_eq7(VALUE) metamacro_if_eq6(metamacro_dec(VALUE))
#define metamacro_if_eq8(VALUE) metamacro_if_eq7(metamacro_dec(VALUE))
#define metamacro_if_eq9(VALUE) metamacro_if_eq8(metamacro_dec(VALUE))
#define metamacro_if_eq10(VALUE) metamacro_if_eq9(metamacro_dec(VALUE))
#define metamacro_if_eq11(VALUE) metamacro_if_eq10(metamacro_dec(VALUE))
#define metamacro_if_eq12(VALUE) metamacro_if_eq11(metamacro_dec(VALUE))
#define metamacro_if_eq13(VALUE) metamacro_if_eq12(metamacro_dec(VALUE))
#define metamacro_if_eq14(VALUE) metamacro_if_eq13(metamacro_dec(VALUE))
#define metamacro_if_eq15(VALUE) metamacro_if_eq14(metamacro_dec(VALUE))
#define metamacro_if_eq16(VALUE) metamacro_if_eq15(metamacro_dec(VALUE))
#define metamacro_if_eq17(VALUE) metamacro_if_eq16(metamacro_dec(VALUE))
#define metamacro_if_eq18(VALUE) metamacro_if_eq17(metamacro_dec(VALUE))
#define metamacro_if_eq19(VALUE) metamacro_if_eq18(metamacro_dec(VALUE))
#define metamacro_if_eq20(VALUE) metamacro_if_eq19(metamacro_dec(VALUE))

/*---------------------------------------------------------------------------------------------------------*/

#define metamacro_if_eq_recursive0(VALUE) metamacro_concat(metamacro_if_eq_recursive0_, VALUE)

#define metamacro_if_eq_recursive0_0(...) __VA_ARGS__ metamacro_consume_
#define metamacro_if_eq_recursive0_1(...) metamacro_expand_
#define metamacro_if_eq_recursive0_2(...) metamacro_expand_
#define metamacro_if_eq_recursive0_3(...) metamacro_expand_
#define metamacro_if_eq_recursive0_4(...) metamacro_expand_
#define metamacro_if_eq_recursive0_5(...) metamacro_expand_
#define metamacro_if_eq_recursive0_6(...) metamacro_expand_
#define metamacro_if_eq_recursive0_7(...) metamacro_expand_
#define metamacro_if_eq_recursive0_8(...) metamacro_expand_
#define metamacro_if_eq_recursive0_9(...) metamacro_expand_
#define metamacro_if_eq_recursive0_10(...) metamacro_expand_
#define metamacro_if_eq_recursive0_11(...) metamacro_expand_
#define metamacro_if_eq_recursive0_12(...) metamacro_expand_
#define metamacro_if_eq_recursive0_13(...) metamacro_expand_
#define metamacro_if_eq_recursive0_14(...) metamacro_expand_
#define metamacro_if_eq_recursive0_15(...) metamacro_expand_
#define metamacro_if_eq_recursive0_16(...) metamacro_expand_
#define metamacro_if_eq_recursive0_17(...) metamacro_expand_
#define metamacro_if_eq_recursive0_18(...) metamacro_expand_
#define metamacro_if_eq_recursive0_19(...) metamacro_expand_
#define metamacro_if_eq_recursive0_20(...) metamacro_expand_

#define metamacro_if_eq_recursive1(VALUE) metamacro_if_eq_recursive0(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive2(VALUE) metamacro_if_eq_recursive1(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive3(VALUE) metamacro_if_eq_recursive2(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive4(VALUE) metamacro_if_eq_recursive3(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive5(VALUE) metamacro_if_eq_recursive4(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive6(VALUE) metamacro_if_eq_recursive5(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive7(VALUE) metamacro_if_eq_recursive6(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive8(VALUE) metamacro_if_eq_recursive7(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive9(VALUE) metamacro_if_eq_recursive8(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive10(VALUE) metamacro_if_eq_recursive9(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive11(VALUE) metamacro_if_eq_recursive10(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive12(VALUE) metamacro_if_eq_recursive11(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive13(VALUE) metamacro_if_eq_recursive12(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive14(VALUE) metamacro_if_eq_recursive13(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive15(VALUE) metamacro_if_eq_recursive14(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive16(VALUE) metamacro_if_eq_recursive15(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive17(VALUE) metamacro_if_eq_recursive16(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive18(VALUE) metamacro_if_eq_recursive17(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive19(VALUE) metamacro_if_eq_recursive18(metamacro_dec(VALUE))
#define metamacro_if_eq_recursive20(VALUE) metamacro_if_eq_recursive19(metamacro_dec(VALUE))

/*---------------------------------------------------------------------------------------------------------*/


#endif /* metamacros_h */
