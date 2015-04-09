#include "AddDialog.h"

AddDialog::AddDialog() :
		IDialog(tr("Create new node")), _nameText(nullptr), _typeText(nullptr), _conditionText(nullptr), _group(
				nullptr) {
}

AddDialog::~AddDialog() {
	delete _nameText;
	delete _typeText;
	delete _conditionText;
	delete _group;
}

void AddDialog::addMainWidgets(QBoxLayout& layout) {
	_group = new QGroupBox(tr("Node"));
	QVBoxLayout *boxLayout = new QVBoxLayout;
	_nameText = new QLineEdit("NewNode");
	boxLayout->addWidget(_nameText);
	_typeText = new QLineEdit("PrioritySelector");
	boxLayout->addWidget(_typeText);
	_conditionText = new QLineEdit("True");
	boxLayout->addWidget(_conditionText);
	_group->setLayout(boxLayout);
	layout.addWidget(_group);
}

void AddDialog::onApply() {
	_condition = _conditionText->text();
	_name = _nameText->text();
	_type = _typeText->text();

	close();
}
