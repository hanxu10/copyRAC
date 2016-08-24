//
//  TableViewController.m
//  textViewInCell
//
//  Created by 旭旭 on 16/8/25.
//  Copyright © 2016年 旭旭. All rights reserved.
//

#import "TableViewController.h"
#import "TableViewCell.h"
#import "CellModel.h"
@interface TableViewController ()
@property (nonatomic, strong)NSMutableArray *datas;
@end

@implementation TableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView.estimatedRowHeight = 150;
    self.tableView.rowHeight = UITableViewAutomaticDimension;
    self.tableView.keyboardDismissMode = UIScrollViewKeyboardDismissModeInteractive;
    
    self.datas = [NSMutableArray arrayWithCapacity:10];
    for (int i = 0; i < 50; i++) {
        CellModel *model = [[CellModel alloc]init];
        model.tvstr = [NSString stringWithFormat:@"左边%zd",i];
        model.rightstr = [NSString stringWithFormat:@"右边%zd",i];
        [self.datas addObject:model];
    }

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {

    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {

    return self.datas.count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    TableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"haha" forIndexPath:indexPath];
    cell.uptv.text = [self.datas[indexPath.row] tvstr];
    cell.righttv.text = [self.datas[indexPath.row] rightstr];
    cell.upDidChangedText = ^ (UITableViewCell *cell, NSString *text){
        NSInteger index = [self.tableView indexPathForCell:cell].row;
        CellModel * model = (CellModel *)self.datas[index];
        model.tvstr = text;
    };
    cell.rightDidChangedText = ^ (UITableViewCell *cell, NSString *text){
        NSInteger index = [self.tableView indexPathForCell:cell].row;
        CellModel * model = (CellModel *)self.datas[index];
        model.rightstr = text;
    };
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
}

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
