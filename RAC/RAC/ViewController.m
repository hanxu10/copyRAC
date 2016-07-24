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


@end
