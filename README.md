<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://i.imgur.com/6wj0hh6.jpg" alt="Project logo"></a>
</p>

<h3 align="center">Qt5-Client-Sockets</h3>


<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
<!-- [![GitHub Issues](https://img.shields.io/github/issues/kylelobo/The-Documentation-Compendium.svg)](https://github.com/kylelobo/The-Documentation-Compendium/issues) -->
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/kylelobo/The-Documentation-Compendium.svg)](https://github.com/kylelobo/The-Documentation-Compendium/pulls)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---


## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Authors](#authors)
- [Screenshots](#screenshots)

## 🧐 About <a name = "about"></a>

 Ce projet est un  **Data	Logger** dans lequel nous visualisons et enregistrons des données.

 Nous avons utilisé une carte **Raspberry Pi** et un capteur **bme280** pour lire les données	d’environnement (la température, l'humidité et la pression) et des données de la carte (température et fréquence du cpu). 

 Ce dépôt présente uniquement l'application de visualisation développée en **Qt5**, elle utilise les **sockets** pour la communication entre l’ordinateur et la Raspberry.
 Vous pouvez trouver [ici](https://github.com/2brams/Qt5ClientMQTT)  une autre version avec le protocole **MQTT**. 

## 🏁 Getting Started <a name = "getting_started"></a>

Ces instructions vous permettront d'obtenir une copie du projet opérationnel sur votre machine locale à des fins de développement et de test. 


### Prerequisites

Nous avons besoin des composants Qt AnalogWidgets téléchargeable [ici](https://store.kde.orgp/1/132205/)

```
unzip AnalogWidgets

cd /path/AnalogWidgets

QT_INSTALL_DIR=/opt/Qt5.1.0/5.1.0
QT_QMAKE_DIR=$QT_INSTALL_DIR/gcc_64/bin/
$QT_QMAKE_DIR/qmake -r AnalogWidgets.pro

make

sudo make install
```

## 🔧 Running the tests <a name = "tests"></a>

Nous devons adapter les lignes suivantes dans le fichier SocketTest1.pro

```
LIBS += /path/AnalogWidgets/analogwidgets/libanalogwidgets.a

INCLUDEPATH += /path/AnalogWidgets/analogwidgets/analogwidgets

```

## ✍️ Authors <a name = "authors"></a>

- [@2brams](https://github.com/2brams)



## Screenshots <a name = "screenshots"></a>


![connexion](https://imgur.com/CvILBXz.png)



![visualisation_data](https://imgur.com/qf19R4S.png)


