@echo off
REM Documentation based on PlantUML (http://plantuml.com/de/index)
REM Download "plantuml.jar" from internet and place it directly into your HOME directory
REM Furthermore it is required that JAVA and Graphviz (https://graphviz.gitlab.io/) is installed.
REM To generate the documentation images (PNG) just execute this batch file.
java -jar %HOMEPATH%\plantuml.jar example.txt classes.txt ptmfx.txt
