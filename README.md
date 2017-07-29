# VTK
- Repositório com alguns programas que utilizam a biblioteca Visualization Toolkit (VTK). Mais informações sobre como instalar e configurar o VTK em sua máquina podem ser obtidas em: 
http://www.vtk.org/Wiki/VTK/Configure_and_Build

- Este repositório está hospedado na seguinte URL: 
https://bergolho1337.github.io/VTK/

# Pré-Requisitos:

- Para conseguir executar os programas deste repositório é necessário ter instalado em seu sistema a versão do CMake 2.2.8 ou superior. No Ubuntu para instalar o CMake basta executar:

```sh
$ sudo apt-get install cmake
```

# Executando os programas

- Para compilar e executar os programas deve-se seguir os seguintes passos.
- Criar uma pasta chamada 'build' no caminho do programa caso não esteja criada, pois é nesta pasta que o CMake irá colocar o executável do programa após compilação.
- Compilar o programa com o CMake através dos comandos:

```sh
$ cd build/
$ cmake ..
$ make
```
- Executar o programa a partir do comando
```sh
$ ./<nome_programa>
```

# ATENÇÃO: 

- Lembre-se que a branch 'master' não existe, logo o comando padrão para o push fica:

```sh
$ git add *
$ git commit -m "Message"
$ git push origin gh-pages
```

