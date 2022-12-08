#include "scriptingsystem.h"
#include <QFile>
#include <QDebug>
#include <QJSEngine>
#include "constants.h"
#include "renderwindow.h"

ScriptingSystem::ScriptingSystem()
{

}

/// Function used to run javascript file and change the speed of the player.
void ScriptingSystem::runScript()
{
    QJSEngine scriptEngine;

    QString fileName = "../GEA2020/scriptfile.js";
    QFile scriptFile(fileName);

    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE: " << fileName;

    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();

    scriptEngine.evaluate(contents, fileName);

    //Call scriptfile function
    QJSValue func = scriptEngine.evaluate("carSpeed");
    QJSValue result = func.call();
    qDebug() << result.toNumber() << "\n";
    carSpeed = result.toNumber();
}
