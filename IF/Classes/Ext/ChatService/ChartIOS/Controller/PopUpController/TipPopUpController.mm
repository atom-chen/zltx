//
//  TipPopUpController.m
//  IF
//
//  Created by mzc on 15/4/17.
//
//

#import "TipPopUpController.h"
#import "LanguageManager.h"
#import "LanguageKeys.h"
#import "ChatCellIOS.h"
#import "ServiceInterface.h"
#import "ChatServiceController.h"
#import "UserManager.h"
#import "MsgMessage.h"
#import "ChatServiceCocos2dx.h"
#import "CSMessage.h"

#import "NSUserInfo.h"
@interface TipPopUpController ()
@property (nonatomic, weak) IBOutlet UIButton *yesBtn;
@property (nonatomic, weak) IBOutlet UIButton *noBtn;
@property (nonatomic, weak) IBOutlet UIImageView *backgroundImage;
@property (nonatomic, weak) IBOutlet UILabel *yesStr;
@property (nonatomic, weak) IBOutlet UIImageView *goldImage;
@property (nonatomic, weak) IBOutlet UILabel *goldCount;
@property (weak, nonatomic) IBOutlet UIButton *yesBtn2;



@property (nonatomic, strong) NSString *yes;
@property (nonatomic, strong) NSString *no;
@end

@implementation TipPopUpController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor =[[UIColor blackColor]colorWithAlphaComponent:0.3];
    // Do any additional setup after loading the view from its nib.
    
    self.view.frame = [ UIScreen mainScreen ].bounds;
    
    CGFloat chatFontSize = [ServiceInterface serviceInterfaceSharedManager].chatFontSize;
    UIFont *uf = [UIFont systemFontOfSize:chatFontSize];
    
    //本地化
    self.yes = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CONFIRM];
    self.no = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].BTN_CANCEL];
    
    //9宫格背景图
    UIImage *image = self.backgroundImage.image;
    
    self.backgroundImage.image = [image resizableImageWithCapInsets:UIEdgeInsetsMake(floorf(image.size.height/2), floorf(image.size.width/2), floorf(image.size.height/2), floorf(image.size.width/2))];
    
    self.yesBtn.titleLabel.font = uf;
    self.noBtn.titleLabel.font = uf;
    self.yesBtn2.titleLabel.font = uf;
    
    self.yesStr.font = uf;
    self.goldCount.font = uf;
    
    self.tipText.font = uf;
    
    
    /**设置tip 说明*/
    NSString *tipStr = nil;
    switch(self.tipType)
    {
        case BLOCKTYPE:
            tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_SHIELD_PLAYER :self.cell.cellFrame.chatMessage.name];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            break;
        case RESENDTYPE:
            tipStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_RESEND];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            break;
        case NOHORN:
            {
                //号角    TIP_HORN
                NSString *horn = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_HORN];
                tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_ITEM_NOT_ENOUGH :horn];
                self.tipText.text = tipStr;
                //隐藏确定按钮文字
                [self.yesBtn setTitle:@"" forState:UIControlStateNormal];
                [self.noBtn setTitle:self.no forState:UIControlStateNormal];
                //显示label确定字样
                self.yesStr.text = self.yes;
                self.yesStr.textAlignment = NSTextAlignmentCenter;
                //显示金币图片
                self.goldImage.image = [UIImage imageNamed:@"ui_gold_coin"];
                //显示发送一个广播的金币数额
                int gc = [[ChatServiceController chatServiceControllerSharedManager] radioCount];
                self.goldCount.text = [NSString stringWithFormat:@"%d",gc];
                self.yesBtn2.hidden = YES;
            }
            break;
        case NOTICEPRICE:
            {
                //金币不足 暂时没走这里。如果需要二级弹窗。走这里
                tipStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_CORN_NOT_ENOUGH];
                self.tipText.text = tipStr;
                [self.yesBtn2 setTitle:self.yes forState:UIControlStateNormal];
                self.yesBtn.hidden = YES;
                self.yesStr.hidden = YES;
                self.goldImage.hidden = YES;
                self.goldCount.hidden = YES;
                self.noBtn.hidden = YES;
            }
            break;
        case RADIOTYPE:
        {
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            NSString *horn = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_HORN];
            tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_USEITEM :horn];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
        }
            break;
        case INVITATION:
            tipStr = [LanguageManager languageManager_getLangByKey_replace:[LanguageKeys lkShared].TIP_CHATROOM_INVITE :@"拉萨凉凉的"];
            self.tipText.text = tipStr;
            [self.yesBtn setTitle:self.yes forState:UIControlStateNormal];
            [self.noBtn setTitle:self.no forState:UIControlStateNormal];
            self.yesStr.hidden = YES;
            self.goldImage.hidden = YES;
            self.goldCount.hidden = YES;
            self.yesBtn2.hidden = YES;
            break;
        default:
            ;
    }
    
    
}

-(void) removeSelf
{
    [self removeFromParentViewController];
    [self.view removeFromSuperview];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (IBAction)onClickYesBtn:(UIButton *)sender {
    switch(self.tipType)
    {
        case BLOCKTYPE:
            {
                NSString *uidString = nil;
                NSString *nameString = nil;
                if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                    uidString = self.cell.cellFrame.chatMessage.uid;
                    nameString =self.cell.cellFrame.chatMessage.name;
                }else{
                    uidString = self.message.uid ;
                    NSUserInfo *user = [[UserManager sharedUserManager]gettingUserInfoForMemoryAndDBWithUidString:uidString];
                    if (user ){
                        nameString = user.userName;
                    }else{
                        nameString = uidString;
                    }
                }
                
                [[ChatServiceController chatServiceControllerSharedManager].gameHost block:uidString:nameString];
                //[[ServiceInterface serviceInterfaceSharedManager] flyHint:@"" :@"" :@"屏蔽成功" :0.0 :0.0 :true];
                [self removeSelf];
            }
            break;
        case RESENDTYPE:
            {
                
                if (self.cell) {
                    if(self.cell.cellFrame.chatMessage.post == 6 && self.cell.cellFrame.chatMessage.channelType == 0){
                        if ([self.tipPopUpControllerDelegate respondsToSelector:@selector(resendRadioByMsg:)]){
                            [self.tipPopUpControllerDelegate resendRadioByMsg:self.cell];
                        }else{
                            [[ChatServiceController chatServiceControllerSharedManager].gameHost sendRadio:self.cell.cellFrame.chatMessage];
                        }
                        
                        [self removeSelf];
                    }else{
                        [self.cell exitResetSend];
                    }
                    
                    [self removeSelf];
                }
            }
            break;
        case NOHORN:
            {
                //判断金币是否足够
                if([[ChatServiceController chatServiceControllerSharedManager] isSatisfySendRadio]){
                    
                    if  (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                        if ([self.tipPopUpControllerDelegate respondsToSelector:@selector(onClickButtonSendRadio2WithMessage:andWithTiPView:)]){
                            
                                [self.tipPopUpControllerDelegate onClickButtonSendRadio2WithMessage:self.message andWithTiPView:self];
                        }
                          [self removeSelf];
                    }else{
                        if ([self.tipPopUpControllerDelegate respondsToSelector:@selector(onClickButtonSendRadio2:)]){
                            if (self.chatMessage) {
                                [self.tipPopUpControllerDelegate onClickButtonSendRadio2:self.chatMessage];
                            }
                        }else{
                           
                            //NSDate *datenow = [NSDate date];
                            //NSString *timeSp = [NSString stringWithFormat:@"%d", (long)[datenow timeIntervalSince1970]];
                            ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:self.chatMessage];
                            [[MsgMessage msgMessageShared] addChatMsgList:self.chatMessage];
                            [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController refreshDisplay:cellFrame];
                            [[ChatServiceController chatServiceControllerSharedManager] sendNotice:self.chatMessage.msg :200011 :true :[NSString stringWithFormat:@"%ld",self.chatMessage.sendLocalTime]];
                        }
                        
                    }
                    [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioMoney = FALSE;
                    [self removeSelf];

                    
               }else{
                    NSLog(@"金币不足");
                    NSString *tipStr = [LanguageManager languageManager_getLangByKey:[LanguageKeys lkShared].TIP_CORN_NOT_ENOUGH];
                    self.tipText.text = tipStr;
                    [self.yesBtn2 setTitle:self.yes forState:UIControlStateNormal];
                    self.yesBtn.hidden = YES;
                    self.yesStr.hidden = YES;
                    self.goldImage.hidden = YES;
                    self.goldCount.hidden = YES;
                    self.noBtn.hidden = YES;
                    self.yesBtn2.hidden = NO;
                    self.tipType = NOTICEPRICE;
                }
            }
            break;
        case NOTICEPRICE:
            {
                [self removeSelf];
            }
            break;
        case RADIOTYPE:
        {
 
                if (ChatServiceCocos2dx::CS_CountryChat_OC_Native_New) {
                    if (ChatServiceCocos2dx::CS_ChatCell_OC_Native_New){
                          if (self.message){
                                if  ([self.tipPopUpControllerDelegate respondsToSelector:@selector(onClickButtonSendRadioWithMessage:andWithTiPView:)]){
                                    [self.tipPopUpControllerDelegate onClickButtonSendRadioWithMessage:self.message andWithTiPView:self];
                                }
                          }
                    }else{
                         if (self.chatMessage){
                            if ([self.tipPopUpControllerDelegate respondsToSelector:@selector(onClickButtonSendRadio:)] ) {
                                [self.tipPopUpControllerDelegate onClickButtonSendRadio:self.chatMessage];
                            }
                         }
                    }
                    
                }else{
                    //            NSDate *datenow = [NSDate date];
                    //            NSString *timeSp = [NSString stringWithFormat:@"%d", (long)[datenow timeIntervalSince1970]];
                    ChatCellFrame *cellFrame=[[ChatCellFrame alloc]init:self.chatMessage];
                    [[MsgMessage msgMessageShared] addChatMsgList:self.chatMessage];
                    [[ServiceInterface serviceInterfaceSharedManager].chatViewController.countriesTableViewController refreshDisplay:cellFrame];
                    [[ChatServiceController chatServiceControllerSharedManager] sendNotice:self.chatMessage.msg :200011 :false :[NSString stringWithFormat:@"%ld",self.chatMessage.sendLocalTime]];
                }
                [ServiceInterface serviceInterfaceSharedManager].isFirstDeductRadioCount = FALSE;
                [self removeSelf];
 
        }
            break;

        case INVITATION:
        {
 
        }
        break;
        default:
        ;
    }
    
    
}
- (IBAction)onClickNoBtn:(UIButton *)sender {
    [self removeSelf];
}

@end
