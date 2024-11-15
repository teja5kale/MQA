#include <QFileDialog>
#include <QGridLayout>
#include <QDebug>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "DataWriter.h"
#include "QualityAnalysis.h"
#include "ModifiedTriangulation.h"
#include "ModifiedTriangle.h"

Visualizer::Visualizer(QWidget* parent) : QMainWindow(parent)
{
    setupUi();
    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
}

Visualizer::~Visualizer()
{
}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    openglWidgetInput = new OpenGlWidget(this);
    progressBar = new QProgressBar(this);
    containerWidget = new QWidget(this);
    firstCheckBox = new QCheckBox("Bounding Box", containerWidget);
    secondCheckBox = new QCheckBox("Aspect Ratio", containerWidget);
    thirdCheckBox = new QCheckBox("Orthogonality", containerWidget);

    param1textbox = new QTextEdit("", containerWidget);
    param2textbox = new QTextEdit("", containerWidget);
    param3textbox = new QTextEdit("", containerWidget);
    param4textbox = new QTextEdit("", containerWidget);
    param5textbox = new QTextEdit("", containerWidget);
    param6textbox = new QTextEdit("", containerWidget);
    param7textbox = new QTextEdit("", containerWidget);

    param1textbox->setReadOnly(true);
    param2textbox->setReadOnly(true);
    param3textbox->setReadOnly(true);
    param4textbox->setReadOnly(true);
    param5textbox->setReadOnly(true);
    param6textbox->setReadOnly(true);
    param7textbox->setReadOnly(true);

    Parameter1 = createReadOnlyTextEdit("No. of Triangles", containerWidget);
    Parameter2 = createReadOnlyTextEdit("Surface Area (sq.unit)", containerWidget);
    Parameter3 = createReadOnlyTextEdit("Triangle Density", containerWidget);
    Parameter4 = createReadOnlyTextEdit("Object Length (unit)", containerWidget);
    Parameter5 = createReadOnlyTextEdit("No. of Vertices", containerWidget);
    Parameter6 = createReadOnlyTextEdit("Object Height (unit)", containerWidget);
    Parameter7 = createReadOnlyTextEdit("Object Breadth (unit)", containerWidget);

    QString buttonStyle = "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "    font-size: 16px;"
        "    border-radius: 5px;"
        "    border: 2px solid black;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3e8e41;"
        "}";

    QString widgetStyle = "QWidget {"
        "    background-color: #333;"
        "    color: #f5f5f5;"
        "}";

    QString containerStyle = "background-color: white; color: black;";

    loadFile->setStyleSheet(buttonStyle);
    this->setStyleSheet(widgetStyle);
    containerWidget->setStyleSheet(containerStyle);

    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);
    containerLayout->addWidget(firstCheckBox);
    containerLayout->addWidget(secondCheckBox);
    containerLayout->addWidget(thirdCheckBox);
    containerLayout->addWidget(Parameter1);
    containerLayout->addWidget(param1textbox);
    containerLayout->addWidget(Parameter2);
    containerLayout->addWidget(param2textbox);
    containerLayout->addWidget(Parameter3);
    containerLayout->addWidget(param3textbox);
    containerLayout->addWidget(Parameter4);
    containerLayout->addWidget(param4textbox);
    containerLayout->addWidget(Parameter5);
    containerLayout->addWidget(param5textbox);
    containerLayout->addWidget(Parameter6);
    containerLayout->addWidget(param6textbox);
    containerLayout->addWidget(Parameter7);
    containerLayout->addWidget(param7textbox);

    QGridLayout* layout = new QGridLayout();
    QWidget* centralWidget = new QWidget(this);

    layout->addWidget(loadFile, 0, 0, 1, 3);
    layout->addWidget(openglWidgetInput, 1, 0, 1, 3);
    layout->addWidget(progressBar, 2, 0, 1, 6);
    layout->addWidget(containerWidget, 0, 3, 2, 3);

    layout->setContentsMargins(20, 20, 20, 20);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void Visualizer::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("files (*.stl *.obj)"));
    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        triangulation = readFile(inputFilePath);
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
        openglWidgetInput->setData(data);

        QualityAnalysis::QualityAnalysis qualityAnalysis;
        qDebug() << "-----------------------------------------" << qualityAnalysis.drawBoundingBox(triangulation);

        // Update the parameter values after the object is read
        param1Value = triangulation.Triangles.size();  // Example: No. of Triangles
        param2Value = qualityAnalysis.surfaceArea(triangulation);  // Surface Area calculation method
        param3Value = qualityAnalysis.triangleDensity(triangulation);  // Triangle Density calculation method
        param4Value = qualityAnalysis.objectLength(triangulation);  // Object Length 
        param5Value = qualityAnalysis.numberOfVertices(triangulation);  // Example: No. of Vertices
        param6Value = qualityAnalysis.objectHeight(triangulation);  // Object Height 
        param7Value = qualityAnalysis.objectBreadth(triangulation);  // Object Breadth

        // Update the textboxes with the new values
        param1textbox->setText(QString::number(param1Value));
        param2textbox->setText(QString::number(param2Value));
        param3textbox->setText(QString::number(param3Value));
        param4textbox->setText(QString::number(param4Value));
        param5textbox->setText(QString::number(param5Value));
        param6textbox->setText(QString::number(param6Value));
        param7textbox->setText(QString::number(param7Value));

    }
}

void Visualizer::onBoundingBoxCheckboxToggled(bool checked)
{
    if (checked)
    {
        // Convert the bounding box triangulation to OpenGL data and display it
        OpenGlWidget::Data boundingBoxData = convertBoundingBoxTriangulatonToGraphcsObject(boundingBoxTriangulation);
        openglWidgetInput->setData(boundingBoxData);
    }
    else
    {
        // Clear bounding box data from OpenGL widget when unchecked
        //openglWidgetInput->clearData(); // You may need to define clearData() to handle clearing
    }
    openglWidgetInput->update();  // Refresh the OpenGL window
}

Triangulation Visualizer::readFile(const QString& filePath)
{
    Triangulation tri;
    if (filePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        STLReader reader;
        reader.read(filePath.toStdString(), tri);
    }
    else if (filePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        OBJReader reader;
        reader.read(filePath.toStdString(), tri);
    }

    ModifiedTriangulation modifiedTriangulation;
    QualityAnalysis::QualityAnalysis qualityAnalysis;

    modifiedTriangulation._minX = qualityAnalysis.minX(tri);
    modifiedTriangulation._minY = qualityAnalysis.minY(tri);
    modifiedTriangulation._minZ = qualityAnalysis.minZ(tri);
    modifiedTriangulation._maxX = qualityAnalysis.maxX(tri);
    modifiedTriangulation._maxY = qualityAnalysis.maxY(tri);
    modifiedTriangulation._maxZ = qualityAnalysis.maxZ(tri);

    std::vector<double> p1 = { modifiedTriangulation._minX, modifiedTriangulation._minY, modifiedTriangulation._minZ };
    std::vector<double> p2 = { modifiedTriangulation._maxX, modifiedTriangulation._minY, modifiedTriangulation._minZ };
    std::vector<double> p3 = { modifiedTriangulation._maxX, modifiedTriangulation._maxY, modifiedTriangulation._minZ };
    std::vector<double> p4 = { modifiedTriangulation._minX, modifiedTriangulation._maxY, modifiedTriangulation._minZ };
    std::vector<double> p5 = { modifiedTriangulation._minX, modifiedTriangulation._minY, modifiedTriangulation._maxZ };
    std::vector<double> p6 = { modifiedTriangulation._maxX, modifiedTriangulation._minY, modifiedTriangulation._maxZ };
    std::vector<double> p7 = { modifiedTriangulation._maxX, modifiedTriangulation._maxY, modifiedTriangulation._maxZ };
    std::vector<double> p8 = { modifiedTriangulation._minX, modifiedTriangulation._maxY, modifiedTriangulation._maxZ };

    boundingBoxTriangulation.push_back({ p1, p2, p3 });
    boundingBoxTriangulation.push_back({ p1, p4, p3 });
    boundingBoxTriangulation.push_back({ p1, p4, p8 });
    boundingBoxTriangulation.push_back({ p1, p5, p8 });
    boundingBoxTriangulation.push_back({ p1, p2, p6 });
    boundingBoxTriangulation.push_back({ p1, p5, p6 });
    boundingBoxTriangulation.push_back({ p2, p7, p6 });
    boundingBoxTriangulation.push_back({ p2, p7, p3 });
    boundingBoxTriangulation.push_back({ p5, p6, p7 });
    boundingBoxTriangulation.push_back({ p5, p8, p7 });
    boundingBoxTriangulation.push_back({ p3, p4, p8 });
    boundingBoxTriangulation.push_back({ p3, p7, p8 });

    qDebug() << "---------------------------------" << modifiedTriangulation._minX;
    qDebug() << "---------------------------------" << modifiedTriangulation._minY;
    qDebug() << "---------------------------------" << modifiedTriangulation._minZ;
    qDebug() << "---------------------------------" << modifiedTriangulation._maxX;
    qDebug() << "---------------------------------" << modifiedTriangulation._maxY;
    qDebug() << "---------------------------------" << modifiedTriangulation._maxZ;

    return tri;
}

void Visualizer::writeFile(const QString& filePath, const Triangulation& tri)
{
    if (filePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        STLWriter writer;
        writer.Write(filePath.toStdString(), tri, progressBar);
    }
    else if (filePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        ObjWriter writer;
        writer.Write(filePath.toStdString(), tri, progressBar);
    }
}

int Vcount = 0;

OpenGlWidget::Data Visualizer::convertBoundingBoxTriangulatonToGraphcsObject(std::vector<std::vector<std::vector<double>>> boundingBoxTriangulation)
{
    OpenGlWidget::Data data;
    int Vcount = 0;
    progressBar->setRange(0, boundingBoxTriangulation.size() - 1);

    for (const auto& triangle : boundingBoxTriangulation)
    {
        for (size_t i = 0; i < 3; ++i)
        {
            data.vertices.push_back(triangle[i][0]);
            data.vertices.push_back(triangle[i][1]);
            data.vertices.push_back(triangle[i][2]);
        }
        Vcount++;
        progressBar->setValue(Vcount);
    }
    return data;
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation)
{
    OpenGlWidget::Data data;
    for each (Triangle triangle in inTriangulation.Triangles)
    {
        for each (Point point in triangle.Points())
        {
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
        }

        Point normal = triangle.Normal();
        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
        }
        progressBar->setValue(Vcount);
        progressBar->setRange(0, inTriangulation.Triangles.size() - 1);
        Vcount++;
    }
    return data;
}

QTextEdit* Visualizer::createReadOnlyTextEdit(const QString& text, QWidget* parent)
{
    QTextEdit* textEdit = new QTextEdit(text, parent);
    textEdit->setReadOnly(true);

    // Center-align the text
    QTextCursor cursor = textEdit->textCursor();
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    textEdit->setTextCursor(cursor);

    // Apply a light background color to both the widget and text area
    QPalette palette = textEdit->palette();
    palette.setColor(QPalette::Base, QColor("#f8f9fa"));  // Light grey background for the text area
    palette.setColor(QPalette::Text, QColor("#333333"));  // Darker text color for readability
    textEdit->setPalette(palette);

    // Apply a basic style
    textEdit->setStyleSheet("QTextEdit {size: 5px; color: #333333; background-color: #f0f0f0; font-size: 14px;}");

    return textEdit;
}


