//
//  ViewController.m
//  RAC
//
//  Created by mac on 16/7/13.
//  Copyright © 2016年 yd. All rights reserved.
//

#import "ViewController.h"
#import "metamacros.h"
@interface ViewController ()<UITableViewDelegate,UITableViewDataSource>
@property (nonatomic, strong)UITableView *tableView;
@property (nonatomic, strong)UIButton *btn;
@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    _tableView = [[UITableView alloc]init];
    _tableView.frame = self.view.bounds;
    _tableView.backgroundColor = [UIColor whiteColor];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    [_tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:@"111"];
    [self.view addSubview:_tableView];
    
    _btn = [UIButton buttonWithType:UIButtonTypeCustom];
    _btn.frame = CGRectMake(100, 100, 100, 100);
    _btn.backgroundColor = [UIColor redColor];
    [self.view addSubview:_btn];
    [_btn addTarget:self action:@selector(sss) forControlEvents:UIControlEventTouchDown];
    metamacro_if_eq(1, 1) (NSLog(@"相当")) (NSLog(@"不相等"));

    @[@"",@"",];

    __weak __typeof__(self) self_weak_ = self;
    
    NSLog(@"");
}

- (NSInteger)sss {
    NSLog(@"sadfasdf");
    return 1;
}
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 100;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"111" forIndexPath:indexPath];
    
    cell.textLabel.text = [NSString stringWithFormat:@"哈哈哈%d",arc4random()];
    return cell;
}
- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    NSLog(@"1111");
}


- (NSString*)TimeformatFromSeconds:(NSInteger)seconds {
    //format of hour
    NSString *str_hour = [NSString stringWithFormat:@"%02d",seconds/3600];
    //format of minute
    NSString *str_minute = [NSString stringWithFormat:@"%02d",(seconds%3600)/60];
    //format of second
    NSString *str_second = [NSString stringWithFormat:@"%02d",seconds%60];
    //format of time
    NSString *format_time = [NSString stringWithFormat:@"%@:%@:%@",str_hour,str_minute,str_second];
    return format_time;
}
@end
