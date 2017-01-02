#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "address.h"
#include "person.h"
#include "date.h"
#include "telephone.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    needSave(false)
{
    ui->setupUi(this);
    srand(time(NULL));
    //Read Settings
    if(!this->ioh.readSettings(this->settings)
            && !this->ioh.readPersons(this->persons)) //首次使用
    {
        QMessageBox::information(this, "欢迎", "欢迎使用2645通讯录，首次使用请设置您的账户信息！",
                                 QMessageBox::Ok, QMessageBox::Ok);
        SettingsDialog *dialog = new SettingsDialog(&(this->settings), this);
        dialog->exec();
        delete dialog;
        ioh.saveSettings(this->settings);
    }
    else    //登陆
    {
        if(this->settings.password != "")
        {
            //Hide MainWindow if needed
            this->setWindowOpacity(0);
            //Validate password if needed
            PasswordValidateDialog *pd = new PasswordValidateDialog(this);
            connect(pd, SIGNAL(onValidatePassword(std::string, PasswordValidateDialog*)),
                    this, SLOT(validatePassword(std::string, PasswordValidateDialog*)));
            connect(pd, SIGNAL(rejected()), this, SLOT(close()));
            pd->exec();
        }
        else
        {
            std::string welcomeMsg = "欢迎，" + (this->settings.username == "" ? "用户！" : this->settings.username + "！");
            QMessageBox::information(this, "登陆成功", QString::fromStdString(welcomeMsg),QMessageBox::Ok, QMessageBox::Ok);
        }
        //Read data
        this->ioh.readPersons(this->persons);
        this->ioh.readPersonGroups(this->persongroups);
        this->ioh.readTelephoneGroups(this->telephonegroups);
        this->ioh.readEmailGroups(this->emailgroups);
    }
    //connections
    connect(ui->exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    //Display using TableView
    this->model = new QStandardItemModel(persons.count(), 3);
    this->setupModel();
    this->setupTableView();
    this->updateTableView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(this->needSave)
    {
        auto rst = QMessageBox::information(this, "提示", "是否保存修改到文件？",
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        switch(rst)
        {
        case QMessageBox::Yes:
            this->on_saveAction_triggered();
            e->accept();
            break;
        case QMessageBox::No:
            e->accept();
            break;
        case QMessageBox::Cancel:
            e->ignore();
            break;
        }
    }
}

void MainWindow::validatePassword(string pwd, PasswordValidateDialog* pd)
{
    if(this->settings.password == pwd)
    {
        pd->hide();
        delete pd;
        std::string welcomeMsg = "欢迎，" + (this->settings.username == "" ? "用户！" : this->settings.username + "！");
        QMessageBox::information(this, "登陆成功", QString::fromStdString(welcomeMsg),QMessageBox::Ok, QMessageBox::Ok);
        this->setWindowOpacity(1);
    }
    else
    {
        QMessageBox::critical(pd, "密码错误", "密码错误，请重试！", QMessageBox::Ok, QMessageBox::Ok);
    }
}

void MainWindow::on_personTableView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid())
    {
        this->needSave = true;
        int id = this->model->item(index.row(), IDColumn)->text().toInt();
        PersonInfoDialog *dialog = new PersonInfoDialog(&(this->persons.findByID(id)->data), &this->persongroups,
                                                        &this->telephonegroups, &this->emailgroups, this);
        connect(dialog, SIGNAL(needUpdatePersonsView()), this, SLOT(updateTableView()));
        dialog->exec();
        delete dialog;
    }
}

void MainWindow::setupModel()
{
    model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("编号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("姓名"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("分组"));
}

void MainWindow::setupTableView()
{
    ui->personTableView->setModel(model);
    ui->personTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->personTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->personTableView->setColumnHidden(0, true);
    ui->personTableView->setSelectionBehavior(QTableView::SelectRows);
}

void MainWindow::generateTestData()
{
    /*PersonGroups IO Test*/
    PersonGroup pg;
    pg.id = 0;
    pg.name = "亲人";
    this->persongroups.insertAsLast(pg);
    pg.id = 1;
    pg.name = "同学";
    this->persongroups.insertAsLast(pg);
    //this->ioh.savePersonGroups(this->persongroups);
    //this->persongroups._elem.clear();
    //this->ioh.readPersonGroups(this->persongroups);

    /*TelephoneGroups IO Test*/
    TelephoneGroup tg;
    tg.id = 0;
    tg.name = "移动电话";
    this->telephonegroups.insertAsLast(tg);
    tg.id = 1;
    tg.name = "固定电话";
    this->telephonegroups.insertAsLast(tg);
    //this->ioh.saveTelephoneGroups(this->telephonegroups);
    //this->telephonegroups._elem.clear();
    //this->ioh.readTelephoneGroups(this->telephonegroups);

    /*EmailGroups IO Test*/
    EmailGroup eg;
    eg.id = 0;
    eg.name = "工作";
    this->emailgroups.insertAsLast(eg);
    eg.id = 1;
    eg.name = "日常";
    this->emailgroups.insertAsLast(eg);
    //this->ioh.saveEmailGroups(this->emailgroups);
    //this->emailgroups._elem.clear();
    //this->ioh.readEmailGroups(this->emailgroups);

    /*Persons IO Test*/
    Person p1, p2, p3, p4;
    p1.id = this->persons.count();
    p1.groupID = 0;
    ++this->persongroups.findByID(p1.groupID)->data.count;
    p1.name = "Cooler";
    p1.address = new Address("China", "Liaoning", "Shenyang", "NEU");
    p1.birthday = new Date(1997, 8, 19);
    Telephone t1("123456", 0);
    ++this->telephonegroups.findByID(t1.groupID)->data.count;
    p1.telephone.push_back(t1);
    this->persons.insertAsLast(p1);

    p2.id = this->persons.count();
    p2.groupID = 1;
    ++this->persongroups.findByID(p2.groupID)->data.count;
    p2.name = "御坂美琴";
    p2.address = new Address("China", "Liaoning", "Shenyang", "NEU");
    p2.birthday = new Date(1997, 1, 1);
    Telephone t2("654321", 0);
    ++this->telephonegroups.findByID(t2.groupID)->data.count;
    p2.telephone.push_back(t2);
    this->persons.insertAsLast(p2);

    p3.id = this->persons.count();
    p3.groupID = -1;
    p3.name = "宫水三叶";
    p3.address = new Address("China", "Liaoning", "Shenyang", "Northeastern University");
    p3.birthday = new Date(1997, 2, 2);
    Telephone t3("2323232323", 1);
    ++this->telephonegroups.findByID(t3.groupID)->data.count;
    p3.telephone.push_back(t3);
    Email e1;
    e1.groupID = -1;
    e1.email = "mituha@miyami.com";
    p3.email.push_back(e1);
    e1.groupID = 1;
    e1.email = "mituha123@miyami.com";
    ++this->emailgroups.findByID(e1.groupID)->data.count;
    p3.email.push_back(e1);
    this->persons.insertAsLast(p3);

    p4.id = this->persons.count();
    p4.name = "Wang";
    p4.groupID = 1;
    ++this->persongroups.findByID(p4.groupID)->data.count;
    p4.address = new Address("China", "Liaoning", "Shenyang", "Northeastern University");
    p4.birthday = new Date(1997, 2, 2);
    Telephone t4("2323232323", -1);
    p4.telephone.push_back(t4);
    this->persons.insertAsLast(p4);

    //this->ioh.savePersons(this->persons);
    //this->persons._elem.clear();
    //this->ioh.readPersons(this->persons);
}

int MainWindow::getRandomNumber(int l, int r)
{
    return rand() % (r - l + 1) + l;
}

string MainWindow::getRandomFamilyName()
{
    //480 Family Names
    string s[] = {"赵","钱","孙","李","周","吴","郑","王","冯","陈",
                  "褚","卫","蒋","沈","韩","杨","朱","秦","尤","许",
                  "何","吕","施","张","孔","曹","严","华","金","魏",
                  "陶","姜","戚","谢","邹","喻","柏","水","窦","章",
                  "云","苏","潘","葛","奚","范","彭","郎","鲁","韦",
                  "昌","马","苗","凤","花","方","俞","任","袁","柳",
                  "酆","鲍","史","唐","费","廉","岑","薛","雷","贺",
                  "倪","汤","滕","殷","罗","毕","郝","邬","安","常",
                  "乐","于","时","傅","皮","卞","齐","康","伍","余",
                  "元","卜","顾","孟","平","黄","和","穆","萧","尹",
                  "姚","邵","湛","汪","祁","毛","禹","狄","米","贝",
                  "明","臧","计","伏","成","戴","谈","宋","茅","庞",
                  "熊","纪","舒","屈","项","祝","董","粱","杜","阮",
                  "蓝","闵","席","季","麻","强","贾","路","娄","危",
                  "江","童","颜","郭","梅","盛","林","刁","钟","徐",
                  "邱","骆","高","夏","蔡","田","樊","胡","凌","霍",
                  "虞","万","支","柯","咎","管","卢","莫","经","房",
                  "裘","缪","干","解","应","宗","宣","丁","贲","邓",
                  "郁","单","杭","洪","包","诸","左","石","崔","吉",
                  "钮","龚","程","嵇","邢","滑","裴","陆","荣","翁",
                  "荀","羊","於","惠","甄","麴","加","封","芮","羿",
                  "储","汲","邴","糜","松","井","段","富","巫","乌",
                  "焦","巴","弓","牧","隗","山","谷","车","侯","宓",
                  "蓬","全","郗","班","仰","秋","仲","伊","宫","宁",
                  "仇","栾","暴","甘","钭","厉","戎","祖","武","符",
                  "刘","景","詹","束","龙","叶","幸","司","韶","郜",
                  "黎","蓟","薄","印","宿","白","怀","蒲","台","从",
                  "鄂","索","咸","籍","赖","卓","蔺","屠","胥","能",
                  "苍","双","闻","莘","党","翟","谭","贡","劳","逄",
                  "姬","申","扶","堵","冉","宰","郦","雍","郤","璩",
                  "桑","桂","濮","牛","寿","通","边","扈","燕","冀",
                  "郏","浦","尚","农","温","别","庄","晏","柴","瞿",
                  "阎","充","慕","连","茹","习","宦","艾","鱼","容",
                  "向","古","易","慎","戈","廖","庚","终","暨","居",
                  "衡","步","都","耿","满","弘","匡","国","文","寇",
                  "广","禄","阙","东","殴","殳","沃","利","蔚","越",
                  "夔","隆","师","巩","厍","聂","晁","勾","敖","融",
                  "冷","訾","辛","阚","那","简","饶","空","曾","毋",
                  "沙","乜","养","鞠","须","丰","巢","关","蒯","相",
                  "查","后","荆","红","游","竺","权","逯","盖","益",
                  "桓","公","万","俟","司","马","上官","欧阳","夏侯","诸葛",
                  "闻人","东方","赫连","皇甫","尉迟","公羊","澹台","公冶","宗政","濮阳",
                  "淳于","仲孙","太叔","申屠","公孙","乐正","轩辕","令狐","钟离","闾丘",
                  "长孙","慕容","鲜于","宇文","司徒","司空","亓官","司寇","仉督","子车",
                  "颛孙","端木","巫马","公西","漆雕","乐正","壤驷","公良","拓拔","夹谷",
                  "宰父","谷粱","晋楚","闫法","汝鄢","涂钦","段干","百里","东郭","南门",
                  "呼延","妫海","羊舌","微生","岳帅","缑亢","况後","有琴","梁丘","左丘",
                  "东门","西门","商牟","佘佴","伯赏","南宫","墨哈","谯笪","年爱","阳佟"};
    return s[this->getRandomNumber(0, 479)];
}

string MainWindow::getRandomMaleFirstName()
{
    //403 First Names
    string s[] = {"安邦","安福","安歌","安国","安和","安康","安澜","安民","安宁","安平","安然","安顺",
                  "安翔","安晏","安宜","安怡","安易","安志","昂然","昂雄",
                  "宾白","宾鸿","宾实","彬彬","彬炳","彬郁","斌斌","斌蔚","滨海","波光","波鸿","波峻","波涛","博瀚","博超","博达","博厚","博简","博明","博容","博赡","博涉","博实","博涛",
                  "博文","博学","博雅","博延","博艺","博易","博裕","博远",
                  "才捷","才良","才艺","才英","才哲","才俊","成和","成弘","成化","成济","成礼","成龙","成仁","成双","成天","成文","成业","成益","成荫","成周","承安","承弼","承德","承恩","承福","承基","承教","承平","承嗣","承天","承望","承宣","承颜","承业","承悦","承允",
                  "承运","承载","承泽","承志",
                  "德本","德海","德厚","德华","德辉","德惠","德容","德润","德寿","德水","德馨","德曜",
                  "德业","德义","德庸","德佑","德宇","德元","德运","德泽","德明",
                  "飞昂","飞白","飞飙","飞掣","飞尘","飞沉","飞驰","飞光","飞翰","飞航","飞翮","飞鸿","飞虎","飞捷","飞龙","飞鸾","飞鸣","飞鹏","飞扬","飞文","飞翔","飞星","飞翼","飞英",
                  "飞宇","飞羽","飞雨","飞语","飞跃","飞章","飞舟","风华","丰茂","丰羽",
                  "刚豪","刚洁","刚捷","刚毅","高昂","高岑","高畅","高超","高驰","高达","高澹","高飞","高芬","高峯","高峰","高歌","高格","高寒","高翰","高杰","高洁","高峻","高朗","高丽",
                  "高邈","高旻","高明","高爽","高兴","高轩","高雅","高扬","高阳","高义","高谊","高逸","高懿","高原","高远","高韵","高卓","光赫","光华","光辉","光济","光霁","光亮","光临","光明","光启","光熙","光耀","光誉","光远","国安","国兴","国源","冠宇","冠玉",
                  "晗昱","晗日","涵畅","涵涤","涵亮","涵忍","涵容","涵润","涵涵","涵煦","涵蓄","涵衍","涵意","涵映","涵育","翰采","翰池","翰飞","翰海","翰翮","翰林","翰墨","翰学","翰音","瀚玥","翰藻","瀚海","瀚漠","昊苍","昊昊","昊空","昊乾","昊穹","昊然","昊然","昊天","昊焱","昊英","浩波","浩博","浩初","浩大","浩宕","浩荡","浩歌","浩广","浩涆","浩瀚","浩浩","浩慨","浩旷","浩阔","浩漫","浩淼","浩渺","浩邈","浩气","浩然","浩穰","浩壤","浩思","浩言","皓轩","和蔼","和安","和璧","和昶","和畅","和风","和歌","和光","和平","和洽","和惬","和顺","和硕","和颂","和泰","和悌","和通","和同","和煦","和雅","和宜","和怡","和玉","和裕","和豫","和悦","和韵","和泽","和正","和志","鹤轩","弘博","弘大","弘方","弘光","弘和","弘厚","弘化","弘济","弘阔","弘亮","弘量","弘深","弘盛","弘图","弘伟","弘文","弘新","弘雅","弘扬","弘业","弘义","弘益","弘毅","弘懿","弘致","弘壮","宏伯","宏博","宏才","宏畅","宏达","宏大","宏放","宏富","宏峻","宏浚","宏恺","宏旷","宏阔","宏朗","宏茂","宏邈","宏儒","宏深","宏胜","宏盛","宏爽","宏硕","宏伟","宏扬","宏义","宏逸","宏毅","宏远","宏壮","鸿宝","鸿波","鸿博","鸿才","鸿彩","鸿畅","鸿畴","鸿达","鸿德","鸿飞","鸿风","鸿福","鸿光","鸿晖","鸿朗","鸿文","鸿熙","鸿羲","鸿禧","鸿信","鸿轩","鸿煊","鸿煊","鸿雪","鸿羽","鸿远","鸿云","鸿运","鸿哲","鸿祯","鸿振","鸿志","鸿卓","华奥","华采","华彩","华灿","华藏","华池","华翰","华皓","华晖","华辉",
                  "华茂","华美","华清","华荣","华容"};
    return s[this->getRandomNumber(0, 402)];
}

string MainWindow::getRandomFemailFirstName()
{
    //499 First Names
    string s[] = {"梦琪","之桃","慕青","尔岚","初夏","沛菡","傲珊","曼文","乐菱","惜文","香寒","新柔","语蓉","海安","夜蓉","涵柏","水桃","醉蓝","语琴","从彤","傲晴","语兰","又菱","碧彤","元霜","怜梦","紫寒","妙彤","曼易","南莲","紫翠","雨寒","易烟","如萱","若南","寻真","晓亦","向珊","慕灵","以蕊","映易","雪柳","海云","凝天","沛珊","寒云","冰旋","宛儿","绿真","晓霜","碧凡","夏菡","曼香","若烟","半梦","雅绿","冰蓝","灵槐","平安","书翠","翠风","代云","梦曼","幼翠","听寒","梦柏","醉易","访旋","亦玉","凌萱","访卉","怀亦","笑蓝","靖柏","夜蕾","冰夏","梦松","书雪","乐枫","念薇","靖雁","从寒","觅波","静曼","凡旋","以亦","念露","芷蕾","千兰","新波","代真","新蕾","雁玉","冷卉","紫山","千琴","傲芙","盼山","怀蝶","冰兰","山柏","翠萱","问旋","白易","问筠","如霜","半芹","丹珍","冰彤","亦寒","之瑶","冰露","尔珍","谷雪","乐萱","涵菡","海莲","傲蕾","青槐","易梦","惜雪","宛海","之柔","夏青","亦瑶","妙菡","紫蓝","幻柏","元风","冰枫","访蕊","芷蕊","凡蕾","凡柔","安蕾","天荷","含玉","书兰","雅琴","书瑶","从安","夏槐","念芹","代曼","幻珊","谷丝","秋翠","白晴","海露","代荷","晓兰","白卉","语山","冷珍","秋翠","夏柳","如之","忆南","书易","翠桃","寄瑶","如曼","问柳","幻桃","又菡","醉蝶","亦绿","诗珊","听芹","新之","易巧","念云","晓灵","静枫","夏蓉","冷雁","飞柏","天真","翠琴","寄真","秋荷","代珊","初雪","雅柏","怜容","如风","南露","紫易","冰凡","海雪","语蓉","碧玉","语风","凝梦","从雪","白枫","傲云","白梅","念露","慕凝","雅柔","盼柳","半青","从霜","怀柔","怜晴","夜蓉","代双","以南","若菱","芷文","南晴","梦寒","初翠","灵波","问夏","惜海","亦旋","沛芹","幼萱","白凝","初露","迎海","绮玉","凌香","寻芹","秋柳","尔白","映真","含雁","寒松","寻雪","青烟","问蕊","灵阳","雪巧","丹萱","凡双","孤萍","紫菱","寻凝","傲柏","傲儿","友容","灵枫","尔丝","曼凝","若蕊","问丝","思枫","水卉","问梅","念寒","诗双","翠霜","夜香","寒蕾","凡阳","冷玉","平彤","语薇","幻珊","紫夏","凌波","芷蝶","丹南","之双","凡波","思雁","白莲","从菡","如容","采柳","沛岚","惜儿","夜玉","水儿","半凡","语海","听莲","幻枫","念柏","冰珍","思山","凝蕊","天玉","思萱","向梦","笑南","夏旋","之槐","元灵","以彤","采萱","巧曼","绿兰","平蓝","问萍","绿蓉","靖柏。迎蕾","碧曼","思卉","白柏","妙菡","怜阳","雨柏","雁菡","梦之","又莲","乐荷","寒天","凝琴","书南","映天","白梦","初瑶","平露","含巧","慕蕊","半莲","醉卉","天菱","青雪","雅旋","巧荷","飞丹","若灵","尔云","幻天","诗兰","青梦","海菡","灵槐","忆秋","寒凝","凝芙","绮山","静白","尔蓉","尔冬","映萱","白筠","冰双","访彤","绿柏","夏云","笑翠","晓灵","含双","盼波","以云","怜翠","雁风","之卉","平松","问儿","绿柳","如蓉","曼容","天晴","丹琴","惜天","寻琴","依瑶","涵易","忆灵","从波","依柔","问兰","山晴","怜珊","之云","飞双","傲白","沛春","雨南","梦之","笑阳","代容","友琴","雁梅","友桃","从露","语柔","傲玉","觅夏","晓蓝","新晴","雨莲","凝旋","绿旋","幻香","觅双","冷亦","忆雪","友卉","幻翠","靖柔","寻菱","千风","天蓉","雅青","寄文","代天","惜珊","向薇","冬灵","惜芹","凌青","谷芹","雁桃","映雁","书兰","寄风","访烟","绮晴","傲柔","寄容","以珊","紫雪","芷容","书琴","寻桃","涵阳","怀寒","易云","采蓝","代秋","惜梦","尔烟","谷槐","怀莲","涵菱","水蓝","访冬","半兰","又柔","冬卉","安双","冰岚","香薇","语芹","静珊","幻露","访天","静柏","凌丝","小翠","雁卉","访文","凌文","芷云","思柔","巧凡","慕山","依云","千柳","从凝","安梦","香旋","映天","安柏","平萱","以筠","忆曼","新竹","绮露","觅儿","碧蓉","白竹","飞兰","曼雁","雁露","凝冬","含灵","初阳","海秋","冰双","绿兰","盼易","思松","梦山","友灵","绿竹","灵安","凌柏","秋柔","又蓝","尔竹","天蓝","青枫","问芙","语海","灵珊","凝丹","小蕾","迎夏","水之","飞珍"};
    return s[this->getRandomNumber(0, 498)];
}

string MainWindow::getRandomName()
{
    int gender = this->getRandomNumber(1, 2);
    string name = this->getRandomFamilyName();
    if(gender == 1)
        name += this->getRandomMaleFirstName();
    else
        name += this->getRandomFemailFirstName();
    return name;
}

string MainWindow::getRandomTelephone()
{
    string tel;
    string head[6] = {"133", "131", "155", "138", "130", "159"};
    tel += head[this->getRandomNumber(0, 5)];
    string num[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for(int i = 0; i < 8; ++i)
        tel += num[this->getRandomNumber(0, 9)];
    return tel;
}

void MainWindow::updateTableView()
{
    //clear
    model->removeRows(0, model->rowCount());
    //sort
    this->persons.sort();
    //display
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < persons.count(); ++i, pit = pit->succ)
    {
        model->setItem(i, IDColumn, new QStandardItem(QString::number(pit->data.id)));
        model->item(i, IDColumn)->setTextAlignment(Qt::AlignCenter);
        model->setItem(i, NameColumn, new QStandardItem(QString::fromStdString(pit->data.name)));
        model->item(i, NameColumn)->setTextAlignment(Qt::AlignCenter);
        if(pit->data.groupID != -1)
            model->setItem(i, GroupColumn, new QStandardItem(QString::fromStdString(this->persongroups.findByID(pit->data.groupID)->data.name)));
        else
            model->setItem(i, GroupColumn, new QStandardItem(QString::fromUtf8("未分组")));
        model->item(i, GroupColumn)->setTextAlignment(Qt::AlignCenter);
    }
    //resize
    ui->personTableView->resizeColumnsToContents();
    //update count
    this->updateCount();
    this->ui->personsCountLabel->setText(QString::number(this->persons.count()));
}

void MainWindow::updateCount()
{
    //Clean persongroups count
    ListNodePosi(PersonGroup) pgit = this->persongroups._elem.first();
    for(int i = 0; i < this->persongroups.count(); ++i, pgit = pgit->succ)
        pgit->data.count = 0;
    //Clean telephonegroups count
    ListNodePosi(TelephoneGroup) tgit = this->telephonegroups._elem.first();
    for(int i = 0; i < this->telephonegroups.count(); ++i, tgit = tgit->succ)
        tgit->data.count = 0;
    //Clean emailgroups count
    ListNodePosi(EmailGroup) egit = this->emailgroups._elem.first();
    for(int i = 0; i < this->emailgroups.count(); ++i, egit = egit->succ)
        egit->data.count = 0;
    //Update count
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        if(pit->data.groupID != -1)
            ++this->persongroups.findByID(pit->data.groupID)->data.count;
        for(vector<Telephone>::iterator it = pit->data.telephone.begin();
            it != pit->data.telephone.end(); ++it)
        {
            if(it->groupID != -1)
                ++this->telephonegroups.findByID(it->groupID)->data.count;
        }
        for(vector<Email>::iterator it = pit->data.email.begin();
            it != pit->data.email.end(); ++it)
        {
            if(it->groupID != -1)
                ++this->emailgroups.findByID(it->groupID)->data.count;
        }
    }
}

void MainWindow::on_pushButton_5_clicked()  //delete
{
    if(ui->personTableView->currentIndex().row() == -1)
        return;
    //Get selected row
    int selectedRow = ui->personTableView->currentIndex().row();
    //Get person's id
    int id = this->model->item(selectedRow, IDColumn)->text().toInt();
    //Get pointer
    ListNodePosi(Person) p =this->persons.findByID(id);
    //Confirm
    QString name = this->model->item(selectedRow, NameColumn)->text();
    if(QMessageBox::information(this, "确认删除", "确认要删除联系人" + name + "吗？",
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        //Remove and update
        this->needSave = true;
        this->persons.remove(p);
        this->updateTableView();
    }
}

void MainWindow::onAddnewperson(Person *newPerson)
{
    this->needSave = true;
    newPerson->id = ++this->persons.maxID;
    this->persons.insertAsLast(*newPerson);
    delete newPerson;
    this->updateTableView();
}

void MainWindow::on_newPushButton_clicked()
{
    PersonInfoDialog *dialog = new PersonInfoDialog(&this->persongroups, &this->telephonegroups,
                                                    &this->emailgroups, this);
    connect(dialog, SIGNAL(addNewPerson(Person*)), this, SLOT(onAddnewperson(Person*)));
    dialog->exec();
    delete dialog;
}

void MainWindow::on_saveAction_triggered()
{
    bool flag = true;
    flag &= this->ioh.savePersons(this->persons);
    flag &= this->ioh.savePersonGroups(this->persongroups);
    flag &= this->ioh.saveTelephoneGroups(this->telephonegroups);
    flag &= this->ioh.saveEmailGroups(this->emailgroups);
    flag &= this->ioh.saveSettings(this->settings);
    if(flag)
    {
        QMessageBox::information(this, "提示", "保存成功！", QMessageBox::Ok, QMessageBox::Ok);
        this->needSave = false;
    }
    else
        QMessageBox::critical(this, "错误", "保存失败，请重试！", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_personGroupsManageAction_triggered()
{
    this->needSave = true;
    PersonGroupsDialog *dialog = new PersonGroupsDialog(&(this->persongroups), false, this);
    connect(dialog, SIGNAL(cleanPersonGroup(PersonGroup*)), this, SLOT(onCleanPersonGroup(PersonGroup*)));
    connect(dialog, SIGNAL(clearPersonGroup(PersonGroup*)), this, SLOT(onClearPersonGroup(PersonGroup*)));
    dialog->exec();
    this->updateTableView();
    delete dialog;
}

void MainWindow::on_telGroupsManageAction_triggered()
{
    this->needSave = true;
    TelephoneGroupsDialog *dialog = new TelephoneGroupsDialog(&(this->telephonegroups), false, this);
    connect(dialog, SIGNAL(cleanTelephoneGroup(TelephoneGroup*)), this, SLOT(onCleanTelephoneGroup(TelephoneGroup*)));
    dialog->exec();
    this->updateTableView();
    delete dialog;
}

void MainWindow::on_emailGroupsManageAction_triggered()
{
    this->needSave = true;
    EmailGroupsDialog *dialog = new EmailGroupsDialog(&(this->emailgroups), false, this);
    connect(dialog, SIGNAL(cleanEmailGroup(EmailGroup*)), this, SLOT(onCleanEmailGroup(EmailGroup*)));
    dialog->exec();
    this->updateTableView();
    delete dialog;
}

void MainWindow::on_settingsManageAction_triggered()
{
    SettingsDialog *dialog = new SettingsDialog(&(this->settings), this);
    dialog->exec();
    this->ioh.saveSettings(this->settings);
    delete dialog;
}

void MainWindow::onCleanEmailGroup(EmailGroup *eg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        for(vector<Email>::iterator it = pit->data.email.begin();
            it != pit->data.email.end(); ++it)
        {
            if(it->groupID == eg->id)
                it->groupID = -1;
        }
    }
}

void MainWindow::onCleanTelephoneGroup(TelephoneGroup *tg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        for(vector<Telephone>::iterator it = pit->data.telephone.begin();
            it != pit->data.telephone.end(); ++it)
        {
            if(it->groupID == tg->id)
                it->groupID = -1;
        }
    }
}

void MainWindow::onCleanPersonGroup(PersonGroup *pg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    for(int i = 0; i < this->persons.count(); ++i, pit = pit->succ)
    {
        if(pit->data.groupID == pg->id)
            pit->data.groupID = -1;
    }
    this->updateTableView();
}

void MainWindow::onClearPersonGroup(PersonGroup *pg)
{
    ListNodePosi(Person) pit = this->persons._elem.first();
    ListNodePosi(Person) q = NULL;
    int cnt = this->persons.count();
    for(int i = 0; i < cnt; ++i, pit = q)
    {
        q = pit->succ;
        if(pit->data.groupID == pg->id)
            this->persons.remove(pit);
    }
    this->updateTableView();
}

void MainWindow::on_pushButton_clicked()
{
    QString keyword = ui->keywordLineEdit->text();
    for(int i = 0; i < this->persons.count(); ++i)
    {
        if(!this->model->item(i, NameColumn)->text().contains(keyword))
            ui->personTableView->setRowHidden(i, true);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->keywordLineEdit->clear();
    for(int i = 0; i < this->persons.count(); ++i)
        ui->personTableView->setRowHidden(i, false);
}

void MainWindow::on_keywordLineEdit_textChanged(const QString &arg1)
{
    for(int i = 0; i < this->persons.count(); ++i)
        ui->personTableView->setRowHidden(i, false);
    if(ui->keywordLineEdit->text().count() != 0)
        this->on_pushButton_clicked();
}

void MainWindow::on_pushButton_3_clicked()
{
    PersonGroupsDialog *dialog = new PersonGroupsDialog(&(this->persongroups), true, this);
    connect(dialog, SIGNAL(selectPersonGroup(PersonGroup*)), this, SLOT(onPersonGroupSelect(PersonGroup*)));
    dialog->exec();
    delete dialog;
}

void MainWindow::onPersonGroupSelect(PersonGroup *pg)
{
    for(int i = 0; i < this->persons.count(); ++i)
        ui->personTableView->setRowHidden(i, false);
    QString keyword = QString::fromStdString(pg->name);
    ui->groupFilterLabel->setText(keyword);
    for(int i = 0; i < this->persons.count(); ++i)
    {
        if(!this->model->item(i, GroupColumn)->text().contains(keyword))
            ui->personTableView->setRowHidden(i, true);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->groupFilterLabel->setText(QString::fromUtf8("全部分组"));
    for(int i = 0; i < this->persons.count(); ++i)
        ui->personTableView->setRowHidden(i, false);
}

void MainWindow::on_loadTestDataAction_triggered()
{
    this->needSave = true;
    this->generateTestData();
    this->updateTableView();
}

void MainWindow::on_aboutAction_triggered()
{
    QMessageBox::information(this, "关于",
                             "数据结构课程设计作品————2645通讯录<br/>本程序能够实现联系人信息管理，并且提供友好的图形交互界面。<br/>使用语言：C++<br/>图形界面库：Qt 5<br/>内部数据存储结构：链表<br/><br/>组长：周子群<br/>组员：赵明阳 龚书杰",
                             QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_genRandomAction_triggered()
{
    //QMessageBox::information(this, "test", QString::fromStdString(this->getRandomTelephone()), QMessageBox::Ok, QMessageBox::Ok);
    if(this->persongroups.count() == 0)
    {
        PersonGroup p1(++this->persongroups.maxID, "测试分组1");
        this->persongroups.insertAsLast(p1);
        PersonGroup p2(++this->persongroups.maxID, "测试分组2");
        this->persongroups.insertAsLast(p2);
    }
    else if(this->persongroups.count() == 1)
    {
        PersonGroup p3(++this->persongroups.maxID, "测试分组2");
        this->persongroups.insertAsLast(p3);
    }
    if(this->telephonegroups.count() == 0)
    {
        TelephoneGroup t1(++this->telephonegroups.maxID, "移动电话");
        this->telephonegroups.insertAsLast(t1);
        TelephoneGroup t2(++this->telephonegroups.maxID, "测试分组");
        this->telephonegroups.insertAsLast(t2);
    }
    else if(this->telephonegroups.count() == 1)
    {
        TelephoneGroup t3(++this->telephonegroups.maxID, "测试分组");
        this->telephonegroups.insertAsLast(t3);
    }
    int n = this->getRandomNumber(3, 10);
    for(int i = 0; i < n; ++i)
    {
        Person p;
        p.id = ++this->persons.maxID;
        p.name = this->getRandomName();
        p.groupID = this->getRandomNumber(0, 1);
        int telCnt = this->getRandomNumber(0, 5);
        for(int j = 0; j < telCnt; ++j)
        {
            Telephone t;
            t.groupID = this->getRandomNumber(0, 1);
            t.tel = this->getRandomTelephone();
            p.telephone.push_back(t);
        }
        p.birthday.year = this->getRandomNumber(1990, 2016);
        p.birthday.month = this->getRandomNumber(1, 12);
        p.birthday.day = this->getRandomNumber(1, 28);
        this->persons.insertAsLast(p);
    }
    this->updateTableView();
}
