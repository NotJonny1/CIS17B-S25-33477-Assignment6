// MainWindow.cpp

/*
 * Congo Storage Management System GUI.
 * The GUI allows users to:
 * - Add new inventory items by entering ID, description, and location.
 * - Look up items by ID and display their details.
 * - Remove items by ID and update the display.
 * - View all items in the system, sorted by description.
 * 
 * Using smart pointers and STL containers in the backend for memory-safe storage.
 * Exceptions - Duplicate entries and not-found items are caught and reported to the user using message dialogs.
 */


#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    // Input for adding new items
    idInput = new QLineEdit();
    descInput = new QLineEdit();
    locInput = new QLineEdit();

    // Input for searching and removing by ID
    findIdInput = new QLineEdit();
    removeIdInput = new QLineEdit();

    // List widget to display the inventory items
    itemList = new QListWidget();

    // Buttons for user actions
    QPushButton *addBtn = new QPushButton("Add Item");
    QPushButton *findBtn = new QPushButton("Find by ID");
    QPushButton *removeBtn = new QPushButton("Remove by ID");

    // Menus
    QMenu *fileMenu = menuBar()->addMenu("File");
    QAction *exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    QMenu *helpMenu = menuBar()->addMenu("Help");
    QAction *aboutAction = helpMenu->addAction("About");
    connect(aboutAction, &QAction::triggered, this, []() {
        QMessageBox::about(nullptr, "About", "Created by [Your Name] for Template Types and Generics Assignment");
    });

    // Main layout for the window
    QWidget *central = new QWidget();
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Add form inputs to layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("ID:", idInput);
    formLayout->addRow("Description:", descInput);
    formLayout->addRow("Location:", locInput);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(addBtn);

    // Layout for finding items
    QHBoxLayout *findLayout = new QHBoxLayout();
    findLayout->addWidget(findIdInput);
    findLayout->addWidget(findBtn);
    mainLayout->addLayout(findLayout);

    // Layout for removing items
    QHBoxLayout *removeLayout = new QHBoxLayout();
    removeLayout->addWidget(removeIdInput);
    removeLayout->addWidget(removeBtn);
    mainLayout->addLayout(removeLayout);

    // Add the item list to the bottom of the layout
    mainLayout->addWidget(itemList);

    // Connect button clicks to the appropriate slots
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddItemClicked);
    connect(findBtn, &QPushButton::clicked, this, &MainWindow::onFindItemClicked);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveItemClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::onAddItemClicked() {
    try {
        std::string id = idInput->text().toStdString();
        std::string desc = descInput->text().toStdString();
        std::string loc = locInput->text().toStdString();

        // Try to add the item to the inventory manager
        manager.addItem(id, desc, loc);

        // Clear input fields after successful addition
        idInput->clear(); descInput->clear(); locInput->clear();

        // Refresh the item list display
        itemList->clear();
        for (auto &item : manager.listItemsByDescription()) {
            itemList->addItem(QString::fromStdString(item->getDescription() + " (" + item->getId() + ")"));
        }
    } catch (const DuplicateItemException &e) {
        // Show a warning if the item already exists
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::onFindItemClicked() {
    try {
        std::string id = findIdInput->text().toStdString();
        auto item = manager.findById(id);

        // Show a message with the found item's details
        QMessageBox::information(this, "Item Found", QString::fromStdString(
            "ID: " + item->getId() + "\nDescription: " + item->getDescription() + "\nLocation: " + item->getLocation()
        ));
    } catch (const ItemNotFoundException &e) {
        // Show an error message if item is not found
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::onRemoveItemClicked() {
    try {
        std::string id = removeIdInput->text().toStdString();

        // Attempt to remove the item from the inventory
        manager.removeItem(id);
        removeIdInput->clear();

        // Refresh the item list display
        itemList->clear();
        for (auto &item : manager.listItemsByDescription()) {
            itemList->addItem(QString::fromStdString(item->getDescription() + " (" + item->getId() + ")"));
        }
    } catch (const ItemNotFoundException &e) {
        // Show an error message if item to remove is not found
        QMessageBox::warning(this, "Error", e.what());
    }
}
