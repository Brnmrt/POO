cmake_minimum_required(VERSION 3.30)
project(ProjetoPOO2024)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProjetoPOO2024 main.cpp
        Elemento.cpp
        Elemento.h
        Item.cpp
        Item.h
        Caravana.cpp
        Caravana.h
        Mapa.cpp
        Mapa.h
        Simulador.cpp
        Simulador.h
        Buffer.cpp
        Buffer.h
        Pixel.cpp
        Pixel.h
        Comando.cpp
        Comando.h
        GestorComandos.cpp
        GestorComandos.h)
