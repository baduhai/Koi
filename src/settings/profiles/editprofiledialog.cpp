//
// Created by da-viper on 04/09/2020.
//

#include "editprofiledialog.h"


EditProfileDialog::EditProfileDialog(QWidget *parent)//cannot pass in a profile here.
	:
	KPageDialog(parent),
	_stylesDialog(nullptr),
	_othersDialog(nullptr),
	_extDialog(nullptr),
	_profile(nullptr)
{
	setWindowTitle("Edit"  " profile");
	this->setMinimumHeight(480);
	//try the one below for a different style, i currently prefer this fn;
	setFaceType(KPageDialog::List);
//	setFaceType(KPageDialog::Tabbed);
	//Adding Pages
	//Styles page
	const QString stylePageName("Styles");
	_stylesDialog = new Ui::StylesDialog();
	auto *stylePageWidget = new QWidget(this);
	_stylesDialog->setupUi(stylePageWidget);

	//put this to make it easier to read
	//point to kpagewidgetitem to add icons.
	KPageWidgetItem *stylesPageItem = this->addPage(stylePageWidget, stylePageName);
	stylesPageItem->setIcon(QIcon::fromTheme("kcolorchooser"));

	//otherPage
	const QString othersPageName("Other");
	_othersDialog = new Ui::OthersDialog();
	auto *othersPageWidget = new QWidget(this);
	_othersDialog->setupUi(othersPageWidget);
	KPageWidgetItem *othersPageItem = addPage(othersPageWidget, othersPageName);
	othersPageItem->setIcon(QIcon::fromTheme("applications-other"));

	//External Page
	const QString extPageName("External");
	_extDialog = new Ui::ExternalDialog();
	auto *extPageWidget = new QWidget(this);
	_extDialog->setupUi(extPageWidget);
	KPageWidgetItem *extPageItem = addPage(extPageWidget, extPageName);
	extPageItem->setIcon(QIcon::fromTheme("preferences"));
	//TODO: enable the External Page when it has been implemented
	extPageItem->setEnabled(false);




    //update pages

    connect(this, &EditProfileDialog::accepted, this, &EditProfileDialog::saveProfile);
    //Styles Page
    connect(_stylesDialog->nameCheckBox, &QCheckBox::stateChanged, this, &EditProfileDialog::enableProfileName);
    connect(_stylesDialog->widgetBox, &QComboBox::currentTextChanged, this, &EditProfileDialog::enableKvantum);

    //Others Page.
    //Wallpaper.
    connect(_othersDialog->wallpaperCheckBox,&QCheckBox::stateChanged, this, &EditProfileDialog::enableWallpaper);
    connect(_othersDialog->wallTypeBox, &QComboBox::currentTextChanged, this, &EditProfileDialog::changeWallType);
    connect(_othersDialog->wallPicBtn, &QPushButton::clicked, this, &EditProfileDialog::selectWallpaper);

    //script
    connect(_othersDialog->scriptCheckBox,
			&QCheckBox::stateChanged,
			_othersDialog->scriptBtn,
			&QPushButton::setEnabled);
    connect(_othersDialog->scriptBtn, &QPushButton::clicked, this, &EditProfileDialog::selectScript);

    setupPage();
}
EditProfileDialog::~EditProfileDialog()
{
    delete _stylesDialog;
    delete _othersDialog;
    delete _extDialog;
    delete _profile;

}
//may seperate this in the future
void EditProfileDialog::updatePages()
{
	Q_ASSERT(_profile);
	//Styles Page.
	//TODO enable later when you can rename profiles
//	if (_profile->name() == "light" || _profile->name() == "dark") {
		_stylesDialog->nameCheckBox->setHidden(true);
//	}
	_stylesDialog->nameTextBox->setText(_profile->name());
	_stylesDialog->plasmaBox->setCurrentText(_profile->getPlasma());
	_stylesDialog->colorBox->setCurrentText(_profile->getColor());
	_stylesDialog->gtkBox->setCurrentText(_profile->getGtk());
	_stylesDialog->widgetBox->setCurrentText(_profile->getWidget());
	_stylesDialog->kvBox->setCurrentText(_profile->getKvantum());
	//Others Page
	_othersDialog->iconBox->setCurrentText(_profile->getIcon());
	_othersDialog->cursorBox->setCurrentText(_profile->getMouse());
	_othersDialog->decorationBox->setCurrentText(_profile->getDecName());
	_othersDialog->scriptCheckBox->setChecked(_profile->getScriptEnabled());
	_othersDialog->scriptBtn->setText(_profile->getScript());

    _othersDialog->wallpaperCheckBox->setChecked(_profile->getWallEnabled());
    if(_othersDialog->wallpaperCheckBox->isChecked()){
        auto wallpaper = _profile->getWallpaper();
        if(wallpaper.toInt() == 0){
             _othersDialog->wallTypeBox->setCurrentText("Image");
             QFileInfo imgInfo(_profile->getWallpaper().toString());
            _othersDialog->wallPicBtn->setText(imgInfo.fileName());
        }else{
            _othersDialog->wallTypeBox->setCurrentText("Unsplash");
            auto index =  _othersDialog->unsplashComboBox->findData(wallpaper.toString());
            _othersDialog->unsplashComboBox->setCurrentIndex(index);
        }
    }


}
void EditProfileDialog::setProfile(Profile *p)
{
    //checks if it points to anything before using .
    _profile = p;
    updatePages();
}
void EditProfileDialog::setupPage()
{
	//Styles Page.
	_stylesDialog->plasmaBox->addItems(Utils::getPlasmaStyles());
	_stylesDialog->colorBox->addItems(Utils::getColorSchemes());
	_stylesDialog->gtkBox->addItems(Utils::getGtkThemes());
	_stylesDialog->widgetBox->addItems(Utils::getWidgetStyles());
	_stylesDialog->kvBox->addItems(Utils::getKvantumStyles());
	//Kvantum Specific
	_stylesDialog->kvBox->setHidden(true);
	_stylesDialog->kvLabel->setHidden(true);

	//Others page
	_othersDialog->iconBox->addItems(Utils::getIcons());
	_othersDialog->cursorBox->addItems(Utils::getCursorThemes());
	_othersDialog->decorationBox->addItems(Utils::getWindowDecorationsStyle());

	_othersDialog->scriptCheckBox->setChecked(false);
	_othersDialog->wallTypeBox->addItems({"Image","Unsplash"});
    _othersDialog->wallTypeBox->setCurrentText("Image");
    _othersDialog->wallpaperCheckBox->setChecked(false);

    setupUnsplash();
}

void EditProfileDialog::saveProfile()
{
	//TODO save only stuff that has been changed.

	//TODO check if the name has been changed so it can be updated and the previous deleted.
	//Style Page
	_profile->setName(_stylesDialog->nameTextBox->text());
	_profile->setPlasma(_stylesDialog->plasmaBox->currentText());
	_profile->setColor(_stylesDialog->colorBox->currentText());
	_profile->setGtk(_stylesDialog->gtkBox->currentText());
	_profile->setWidget(_stylesDialog->widgetBox->currentText());
	auto widgetName = _stylesDialog->widgetBox->currentText();
	if (widgetName == "kvantum" || widgetName == "kvantum-dark") {
		_profile->setKvantum(_stylesDialog->kvBox->currentText());
	}
	//Others
	_profile->setIcon(_othersDialog->iconBox->currentText());
	_profile->setMouse(_othersDialog->cursorBox->currentText());
	_profile->setScriptEnabled(_othersDialog->scriptCheckBox->isChecked());
	_profile->setWallEnabled(_othersDialog->wallpaperCheckBox->isChecked());
	if(!_othersDialog->unsplashComboBox->isHidden()){
	    _profile->setWallpaper(_othersDialog->unsplashComboBox->currentData());
	}

	//Decorations.
	QString decoration(_othersDialog->decorationBox->currentText());
	QList<Decoration> decList = Utils::getWindowDecorations();
	for (const auto &dt : qAsConst(decList)) {
		if (QString::compare(dt.name, decoration, Qt::CaseInsensitive) == 0) {
			_profile->setDecName(dt.name);
			_profile->setLibrary(dt.library);
			_profile->setTheme(dt.theme);
		}
	}

	if (!ProfileManager::instance()->profileExists(_profile->name())) {
		emit addNewProfile(_profile);
	}
	_profile->setConfigPath();
	_profile->setGlobDir();
    // TODO this is meant to be in the controller as i currently don't know how.
	ProfileManager::instance()->addProfile(_profile);
	ProfileManager::instance()->saveProfile(_profile->name());

}
void EditProfileDialog::enableKvantum(const QString &widgetName)
{
	if (widgetName == "kvantum" || widgetName == "kvantum-dark") {
		_stylesDialog->kvBox->setEnabled(true);
		_stylesDialog->kvBox->setHidden(false);
		_stylesDialog->kvLabel->setHidden(false);
	}
	else {
		_stylesDialog->kvBox->setEnabled(false);
		_stylesDialog->kvBox->setHidden(true);
		_stylesDialog->kvLabel->setHidden(true);
	}
}
void EditProfileDialog::enableWallpaper(const bool &isEnabled)
{
    _othersDialog->wallTypeBox->setEnabled(isEnabled);
    _othersDialog->wallPicBtn->setEnabled(isEnabled);
    _othersDialog->unsplashComboBox->setEnabled(isEnabled);
}
void EditProfileDialog::changeWallType(const QString &wallType)
{
    if(wallType == "Image"){
        _othersDialog->unsplashComboBox->setHidden(true);
        _othersDialog->wallPicBtn->setHidden(false);
        _othersDialog->unsplashLabel->setHidden(true);
        _othersDialog->imgLabel->setHidden(false);
    }
    if(wallType == "Unsplash"){
        _othersDialog->unsplashComboBox->setHidden(false);
        _othersDialog->wallPicBtn->setHidden(true);
        _othersDialog->unsplashLabel->setHidden(false);
        _othersDialog->imgLabel->setHidden(true);
    }
}
void EditProfileDialog::enableProfileName(const int &state)
{
	_stylesDialog->nameTextBox->setEnabled(state);
}
void EditProfileDialog::selectScript()
{
    //TODO point to ~/.local/share/koi/script
	_profile->setScript(QFileDialog::getOpenFileName(this,
													 tr("Run Script"),
													 "/home",
													 tr("Script Files(.sh) (*.sh)")));
}
void EditProfileDialog::selectWallpaper()
{
    QFileInfo imgPath;
	//todo add the file format here for wallpaper.
	_profile->setWallpaper(QFileDialog::getOpenFileName(this,
                                                     tr("Choose wallpaper"),
                                                     QDir::homePath()+ "/Pictures",
                                                     tr(".png",".jpg")));
	imgPath= _profile->getWallpaper().toString() ;
	if(imgPath.exists()){
	    _othersDialog->wallPicBtn->setText(imgPath.fileName());
	}
}
void EditProfileDialog::setupUnsplash()
{
    auto box(_othersDialog->unsplashComboBox);
    box->clear();
    box->addItem("4k",133980);
    box->addItem("Ultra Wide", 1339089);
    box->addItem("Background", 1339276);
    box->addItem("Nature", 1065376);
    box->addItem("Pastel", 531563);
    box->addItem("Black", 1101680);
    box->addItem("Ice", 482366);
    box->addItem("Top View", 399194);
    //TODO add more from the doc file but not too much .
}




