#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <cstring>
#include <ctype.h>
#include <cstdlib>
#include <time.h>

#include <sstream>
using namespace std;

struct Usuario{
    string user;
    string password;
}u[10];

struct TipoDeMedicamento{
    int id;
    char tipo[20];
    bool activo;
}t[20];

struct Medicamento{
    int id;
    char codigo[20];
    char nombre[20];
    char droga[20];
    bool activo;
    struct TipoDeMedicamento tdm;
}m[20];

void lectura(int *contUser);
bool verificar(char informacion[50],string *userAct,int contadorUsuarios);
void crearMedicamento(int contadorMedicamento);
void leerMedicamento(int contadorMedicamento);
void crearTipoDeMedicamento(int contadorTipoDeMedicamento);
void leerTipoDeMedicamento(int contadorTipoDeMedicamento);

string addTipo(char tipo[20],int *contTipo,int contadorTipoDeMedicamento,int *dat);
string addMed(char informacion[50],int *contMed,int contadorMedicamento,int contadorTipoDeMedicamento,int *dat);
string compararMed(char nombre[20],char tipo[20],char codigo[20],int contadorMedicamento);
bool compararTipo(char denominacion[20],int contadorTipoDeMedicamento);
string administrarMed(char datos[20],int contadorMedicamento);
string administrarTipo(char datos[20],int contadorTipoDeMedicamento);
string compararCodigo(char codigo[20],int contadorMedicamento);

void escribirArchivoLogInicio(int puertoEscucha);
void escribirArchivoLogIngreso(string usuarioActual);
void escribirArchivoLogCierre(string usuarioActual);
void escribirArchivoUserInicia(string usuarioActual);
void escribirArchivoUserCierra(string usuarioActual);
void escribirArchivoUserAccion(int id,string accion,string usuarioActual);
string transferirArchivo(string usuarioActual);

string modificarMed(char id[20],int contadorMedicamento,int *medAMod);
string pedirDatosMed(char informacion[50],int contadorMedicamento,int contadorTipoDeMedicamento,int medAModificar,struct Medicamento *medAux);
string guardoMed(int medAModificar,struct Medicamento mAux);
string modificarTipo(char id[20],int contadorTipoDeMedicamento,int *tipoAMod);
string pedirDatosTipo(char informacion[50],int contadorTipoDeMedicamento,int tipoAModificar,struct TipoDeMedicamento *tipAux);
string guardoTipo(int tipoAModificar,struct TipoDeMedicamento tAux);

int medio(string datos);
string primeraParte(string datos);
string segundaParte(string datos);

void reescribirArchivoMed(int medAModificar);
void reescribirArchivoTipo(int tipAModificar);
bool corroborarIdMed(int id,int contadorMedicamento);
bool corroborarIdTipo(int id,int contadorTipoDeMedicamento);
int buscarIdTipo(char tipo[20],int contadorTipoDeMedicamento);
bool buscarActivoTipo(char tipo[20],int contadorTipoDeMedicamento);
void sumarContadorDeMed(int contadorMedicamento);
void sumarContadorDeTip(int contadorTipoDeMedicamento);
void leerContadorDeMed(int *conMed);
void leerContadorDeTip(int *conTipo);

void leerYMostraerDatosBinarios(int contadorTipoDeMedicamento,int contadorMedicamento);
void Tip(int contadorTipoDeMedicamento);
void Med(int contadorMedicamento);

int main()
{
    int contadorMedicamento;
    int *contMed=NULL;
    contMed=&contadorMedicamento;

    int contadorTipoDeMedicamento;
    int *contTipo=NULL;
    contTipo=&contadorTipoDeMedicamento;

    string usuarioActual;
    string *userAct=NULL;
    userAct=&usuarioActual;

    int contadorUsuarios=0;
    int *contUser=NULL;
    contUser=&contadorUsuarios;

    int tipoAModificar=0;
    int *tipAMod=NULL;
    tipAMod=&tipoAModificar;

    int medAModificar=0;
    int *medAMod=NULL;
    medAMod=&medAModificar;

    struct TipoDeMedicamento tAux;
    struct TipoDeMedicamento *tipAux=NULL;
    tipAux=&tAux;

    struct Medicamento mAux;
    struct Medicamento *medAux=NULL;
    medAux=&mAux;

    int datoCreado=0;
    int *dat=NULL;
    dat=&datoCreado;

    string datos;
    string parte1;
    string parte2;
    int eleccion;
    bool iniciarConexion=true;
    int intentos=0;
    int puertoEscucha=5010;

    leerContadorDeMed(contMed);
    leerContadorDeTip(contTipo);
    lectura(contUser);

    //leerYMostraerDatosBinarios(contadorTipoDeMedicamento,contadorMedicamento);

    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(puertoEscucha);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);
    char buffer[1024];
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server, &master);
    bool running = true;
    //Intento de utilizar un temporizador pero no funciona con un thread
    //int ReceiveTimeout = 10000;
    //setsockopt(server, SOL_SOCKET, SO_RCVTIMEO, (char*)&ReceiveTimeout, sizeof(ReceiveTimeout));

    while(true)
    {
        fd_set copy = master;
        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
        for (int i = 0; i < socketCount; i++)
        {
            SOCKET sock = copy.fd_array[i];
            if (sock == server)
            {
                iniciarConexion=true;
                eleccion=0;
                int clientAddrSize = sizeof(clientAddr);
                client = accept(server,(SOCKADDR *)&clientAddr, &clientAddrSize);
                cout << "Un cliente se ha conectado." << endl;
                FD_SET(client, &master);
            }
        }
        bool yes=true;
        // bucle para enviar/recibir mensajes
        while(yes){
            //------------------- ENVIO MENSAJE -------------------

            if(iniciarConexion==true){
                strcpy(buffer,"\nIndique el User y Password en un formato (user;password)");
            }
            if(intentos==3){
                strcpy(buffer,"se supero la cantidad maxima de intentos de ingreso");
                intentos=0;
            }
            //ENVIO SEGUN LO QUE SE RECIBIO
            switch(eleccion){
                case 1:{
                    strcpy(buffer,"\n----------Agregar un Tipo De Medicamento----------:\n\nIndique el tipo:");
                break;
                }
                case 2:{
                    strcpy(buffer,"\n\tIngrese la denominacion y si esta activo\n\ten un formato(denominacion,activo):\nDenominacion:\n'c' traera todos los tipos que empiezan con c.\n'jarabe' para traer el tipo jarabe.\n'@' para no aplicar filtro\nActivo:\n's' traera todos los Activos.\n'n' traera todos los Inactivos.\n'@' para no aplicar filtro");
                break;
                }
                case 4:{
                    strcpy(buffer,"\n----------Agregar un Medicamento----------:\n\nIngrese un codigo, nombre comercial, droga y el tipo de medicamento\n\ten un formato(codigo,nombre,droga,tipo):");
                break;
                }
                case 5:{
                    strcpy(buffer,"\nIngrese el nombre comercial y si el tipo de medicamento\n en un formato (nombre,tipo):\nNombre comercial:\n'a' traera todos los medicamentos que empiezan con a.\n'durezol' para traer el medicamento durezol.\n'@' para no aplicar filtro\nTipo de medicamento:\n'c' traera todos los medicamentos de tipo que empiezan con c.\n'jarabe' traera todos los medicamentos de tipo jarabe.\n'@' para no aplicar filtro");
                break;
                }
                case 6:{
                    datos=transferirArchivo(usuarioActual);
                    if(datos.size()>=1024){
                        parte1=primeraParte(datos);
                        parte2=segundaParte(datos);
                        strcpy(buffer,"El archivo de actividades del usuario es muy largo, se dividira en dos mensajes\nPresione cualquier tecla para continuar");
                        eleccion=23;
                    }else{
                        strcpy(buffer,datos.c_str());
                    }
                break;
                }
                case 7:{
                    strcpy(buffer,"\n\t\tSesion cerrada\nIndique el User y Password en un formato (user;password)");
                    escribirArchivoLogCierre(usuarioActual);
                    escribirArchivoUserCierra(usuarioActual);
                    //iniciarConexion=true;
                    //eleccion=0;
                    intentos=0;
                    //yes=false;
                break;
                }
                case 8:{
                    strcpy(buffer,datos.c_str());
                break;
                }
                case 9:{
                    strcpy(buffer,datos.c_str());
                break;
                }
                case 10:{
                    strcpy(buffer,datos.c_str());
                break;
                }
                case 11:{
                    strcpy(buffer,datos.c_str());
                break;
                }
                case 12:{
                    strcpy(buffer,datos.c_str());
                break;
                }
                case 13:{
                    strcpy(buffer,datos.c_str());
                break;
                }
                case 14:{
                    strcpy(buffer,datos.c_str());
                break;
                }
                case 15:{
                    strcpy(buffer,"\nSe ha aceptado la conexion");
                    eleccion=0;
                break;
                }
                case 16:{
                    strcpy(buffer,"La lista de Tipos de Medicamentos es muy larga, se dividira en dos mensajes\nPresione cualquier tecla para continuar");
                    eleccion=17;
                break;
                }
                case 17:{
                    strcpy(buffer,parte1.c_str());
                    eleccion=18;
                break;
                }
                case 18:{
                    strcpy(buffer,parte2.c_str());
                    eleccion=8;
                break;
                }
                case 19:{
                    strcpy(buffer,"La lista de Medicamentos es muy larga, se dividira en dos mensajes\nPresione cualquier tecla para continuar");
                    eleccion=20;
                break;
                }
                case 20:{
                    strcpy(buffer,parte1.c_str());
                    eleccion=21;
                break;
                }
                case 21:{
                    strcpy(buffer,parte2.c_str());
                    eleccion=12;
                break;
                }
                /*case 22:{
                    strcpy(buffer,"El archivo de actividades del usuario es muy largo, se dividira en dos mensajes\nPresione cualquier tecla para continuar");
                    eleccion=23;
                break;
                }*/
                case 23:{
                    strcpy(buffer,parte1.c_str());
                    eleccion=24;
                break;
                }
                case 24:{
                    strcpy(buffer,parte2.c_str());
                    eleccion=0;
                break;
                }
            }
            send(client, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            cout << "Mensaje enviado!" << endl;

            //------------------- RECIBO MENSAJE -------------------

            recv(client, buffer, sizeof(buffer), 0);
            string mensaje(buffer);
            if(mensaje.size()==0){
                cout << "No hay un cliente conectado" << endl;
                system("PAUSE");
                yes=false;
                iniciarConexion=true;
                intentos=0;
                break;
            }
            int id;
            if(iniciarConexion==true){
                if(verificar(buffer,userAct,contadorUsuarios)==true){
                    iniciarConexion=false;
                    escribirArchivoLogIngreso(usuarioActual);
                    escribirArchivoUserInicia(usuarioActual);
                    eleccion=15;
                    intentos=0;
                }else{
                    intentos++;
                }
            }
            //TOMO LOS DATOS DEPENDIENDO QUE SE RECIBA
            switch(eleccion){
                case 1:{
                    datos=addTipo(buffer,contTipo,contadorTipoDeMedicamento,dat);
                    string aux1=datos.substr(0,61);
                    if(strcmp(aux1.c_str(),"\nEl tipo de medicamento se ha creado correctamente. Su id es:")==0){
                        escribirArchivoUserAccion(datoCreado-1,"Creacion Tipo de medicamento",usuarioActual);
                    }
                    eleccion=10;
                break;
                }
                case 2:{
                    bool flag;
                    datos=administrarTipo(buffer,contadorTipoDeMedicamento);
                    if(datos.size()>=1024){
                        parte1=primeraParte(datos);
                        parte2=segundaParte(datos);
                        flag=true;
                        eleccion=16;
                    }
                    if(strcmp(datos.c_str(),"No se encontro ningun resultado para los criterios seleccionados.\nPresione ENTER para continuar")==0){
                        eleccion=10;
                    }else if(flag==false){
                        eleccion=8;
                    }
                break;
                }
                case 4:{
                    datos=addMed(buffer,contMed,contadorMedicamento,contadorTipoDeMedicamento,dat);
                    string aux1=datos.substr(0,53);
                    if(strcmp(aux1.c_str(),"\nEl medicamento se ha creado correctamente. Su id es:")==0){
                        escribirArchivoUserAccion(datoCreado-1,"Creacion Medicamento",usuarioActual);
                    }
                    eleccion=10;
                break;
                }
                case 5:{
                    bool flag;
                    datos=administrarMed(buffer,contadorMedicamento);
                    if(datos.size()>=1024){
                        parte1=primeraParte(datos);
                        parte2=segundaParte(datos);
                        flag=true;
                        eleccion=19;
                    }
                    if(strcmp(datos.c_str(),"No se encontro ningun resultado para los criterios seleccionados.\nPresione ENTER para continuar")==0){
                        eleccion=10;
                    }else if(flag==false){
                        eleccion=12;
                    }
                break;
                }
                case 8:{
                    datos=modificarTipo(buffer,contadorTipoDeMedicamento,tipAMod);
                    if(strcmp(datos.c_str(),"\nNo existe el tipo de medicamento con el id indicado")==0){
                        eleccion=10;
                    }else{
                        eleccion=9;
                    }
                break;
                }
                case 9:{
                    datos=pedirDatosTipo(buffer,contadorTipoDeMedicamento,tipoAModificar,tipAux);
                    if(strcmp(datos.c_str(),"\nEl tipo de medicamento especificado ya existe.")==0){
                        eleccion=10;
                    }else{
                        eleccion=11;
                    }
                break;
                }
                case 11:{
                    if(buffer[0]=='s'){
                        datos=guardoTipo(tipoAModificar,tAux);
                        escribirArchivoUserAccion(tipoAModificar,"Modificacion Tipo de medicamento",usuarioActual);
                    }
                    eleccion=10;
                break;
                }
                case 12:{
                    datos=modificarMed(buffer,contadorMedicamento,medAMod);
                    if(strcmp(datos.c_str(),"\nNo existe el medicamento con el id indicado")==0){
                        eleccion=10;
                    }else{
                        eleccion=13;
                    }
                break;
                }
                case 13:{
                    datos=pedirDatosMed(buffer,contadorMedicamento,contadorTipoDeMedicamento,medAModificar,medAux);
                    if(strcmp(datos.c_str(),"\nSe ha producido algun error en alguno de los datos")==0){
                        eleccion=10;
                    }else{
                        eleccion=14;
                    }
                break;
                }
                case 14:{
                    if(buffer[0]=='s'){
                        datos=guardoMed(medAModificar,mAux);
                        escribirArchivoUserAccion(medAModificar,"Modificacion de Medicamento",usuarioActual);
                    }
                    eleccion=10;
                break;
                }
            }
            if(isdigit(buffer[0])){
                eleccion=atoi(buffer);
            }
            cout << "El Cliente dice: \n"<<buffer<< endl;
            memset(buffer, 0, sizeof(buffer));
        }
    }
}

void crearMedicamento(int contadorMedicamento){
    ofstream archivoB;
    archivoB.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\Medicamento.dat",ios::out | ios::binary);
    if(archivoB.fail()){
        cout<<"No se pudo crear el archivo"<<endl;
        exit(1);
    }
    cout<<"\nPidiendo datos para el Medicamento: "<<endl;
    for(int i=0;i<contadorMedicamento;i++){
        cout<<"\nIndique el id: "<<endl;
        cin>>m[i].id;
        cout<<"\nIndique el codigo: "<<endl;
        cin>>m[i].codigo;
        cout<<"\nIndique el nombre: "<<endl;
        cin>>m[i].nombre;
        cout<<"\nIndique la droga: "<<endl;
        cin>>m[i].droga;
        m[i].activo=true;
        cout<<"\nIndique el id del Tipo: "<<endl;
        cin>>m[i].tdm.id;
        cout<<"\nIndique el tipo del Tipo: "<<endl;
        cin>>m[i].tdm.tipo;
        archivoB.write((char *)&m[i],sizeof(Medicamento));
    }
    archivoB.close();
}

void crearTipoDeMedicamento(int contadorTipoDeMedicamento){
    ofstream archivoB;
    archivoB.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\TipoDeMedicamento.dat",ios::out | ios::binary);
    if(archivoB.fail()){
        cout<<"No se pudo crear el archivo"<<endl;
        exit(1);
    }
    cout<<"\nPidiendo datos para el Tipo De Medicamento: "<<endl;
    for(int i=0;i<contadorTipoDeMedicamento;i++){
        cout<<"\nIndique el id: "<<endl;
        cin>>t[i].id;
        cout<<"\nIndique el tipo: "<<endl;
        cin>>t[i].tipo;
        t[i].activo=true;
        archivoB.write((char *)&t[i],sizeof(TipoDeMedicamento));
    }

    archivoB.close();
}

void lectura(int *contUser){
    ifstream archivo;
    string texto;
    int inicio=1;
    int usuarioNumero=0;

    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\archivo.txt",ios::in);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    while(getline(archivo,texto)){
        *contUser=*contUser+1;
        for(int i=0;i<texto.size();i++){
            if(texto[i]==';'){
                inicio=2;
            }
            switch(inicio){
                case 1:{
                    u[usuarioNumero].user+=texto[i];
                break;
                }
                case 2:{
                    if(texto[i]!=';'){
                        u[usuarioNumero].password+=texto[i];
                    }
                break;
                }
            }
            if(i==texto.size()-1){
                inicio=1;
                usuarioNumero++;
            }
        }
    }
    /*for(int i=0;i<*contUser;i++){
        cout<<"User "<<u[i].user<<endl;
        cout<<"Password "<<u[i].password<<endl;
    }*/
    //cout<<*contUser<<endl;
    archivo.close();
}

bool verificar(char informacion[50],string *userAct,int contadorUsuarios){
    bool flag;
    char usuario[20];
    char contra[20];
    string str(informacion);
    int stop=0;
    int contador1=0;
    int contador2=0;
    int inicio=1;
    for(int i=0;i<str.size();i++){
        if(str[i]==';'){
            inicio++;
        }
        switch(inicio){
            case 1:{
                contador1++;
            break;
            }
            case 2:{
                if(str[i]==';'){
                   stop=i+1;
                }else{
                    contador2++;
                }
            break;
            }
        }
    }
    string aux1=str.substr(0,contador1);
    string aux2=str.substr(stop,contador2);
    strcpy(usuario,aux1.c_str());
    strcpy(contra,aux2.c_str());

    for(int i=0;i<contadorUsuarios;i++){
        if(strcmp(usuario,u[i].user.c_str())==0 && strcmp(contra,u[i].password.c_str())==0){
            flag=true;
            *userAct=u[i].user;
        }
    }
    return flag;
}

void leerMedicamento(int contadorMedicamento){
    ifstream archivoB;
    archivoB.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\Medicamento.dat",ios::in | ios::binary);
    if(archivoB.fail()){
        cout<<"No se pudo crear el archivo"<<endl;
        exit(1);
    }
    for(int i=0;i<contadorMedicamento;i++){
        archivoB.read((char *)&m[i],sizeof(Medicamento));
    }
    archivoB.close();
}

void leerTipoDeMedicamento(int contadorTipoDeMedicamento){
    ifstream archivoB;
    archivoB.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\TipoDeMedicamento.dat",ios::in | ios::binary);
    if(archivoB.fail()){
        cout<<"No se pudo crear el archivo"<<endl;
        exit(1);
    }
    for(int i=0;i<contadorTipoDeMedicamento;i++){
        archivoB.read((char *)&t[i],sizeof(TipoDeMedicamento));
    }
    archivoB.close();
}

string addTipo(char tipo[20],int *contTipo,int contadorTipoDeMedicamento,int *dat){
    leerTipoDeMedicamento(contadorTipoDeMedicamento);
    ofstream archivo;
    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\TipoDeMedicamento.dat",ios::app | ios::binary);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    string mensaje;
    char aux[20];
    for(int i=0;i<20;i++){
        aux[i] = toupper(tipo[i]);
    }
    std::ostringstream ostr;
    if(compararTipo(aux,contadorTipoDeMedicamento)==true){
        mensaje="\nEl tipo de medicamento: ";
        mensaje+=aux;
        mensaje+="\nya existe, por favor ingrese otra denominacion: ";
    }
    else{
        *contTipo=*contTipo+1;
        *dat=*contTipo;
        sumarContadorDeTip(*contTipo);
        t[*contTipo].id=*contTipo;
        strcpy(t[*contTipo].tipo, aux);
        t[*contTipo].activo=true;
        archivo.write((char *)&t[*contTipo],sizeof(TipoDeMedicamento));
        mensaje="\nEl tipo de medicamento se ha creado correctamente. Su id es:";
        ostr << t[*contTipo].id;
        std::string identificador = ostr.str();
        mensaje+=identificador;
    }
    archivo.close();
    return mensaje;
}

string addMed(char informacion[50],int *contMed,int contadorMedicamento,int contadorTipoDeMedicamento,int *dat){
    leerMedicamento(contadorMedicamento);
    ofstream archivo;
    char codigo[20];
    char nombre[20];
    char droga[20];
    char tipo[20];

    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\Medicamento.dat",ios::app | ios::binary);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    string str(informacion);
    int stop1=0;
    int stop2=0;
    int stop3=0;
    int contador1=0;
    int contador2=0;
    int contador3=0;
    int inicio=1;
    for(int i=0;i<str.size();i++){
        if(str[i]==','){
            inicio++;
        }
        switch(inicio){
            case 1:{
                codigo[i]=str[i];
            break;
            }
            case 2:{
                if(str[i]==','){
                    stop1=i+1;
                }else{
                    contador1++;
                }
            break;
            }
            case 3:{
                if(str[i]==','){
                    stop2=i+1;
                }else{
                    contador2++;
                }
            break;
            }
            case 4:{
                if(str[i]==','){
                    stop3=i+1;
                }else{
                    contador3++;
                }
            break;
            }
        }
    }
    string aux1=str.substr(stop1,contador1);
    string aux2=str.substr(stop2,contador2);
    string aux3=str.substr(stop3,contador3);
    strcpy(nombre,aux1.c_str());
    strcpy(droga,aux2.c_str());
    strcpy(tipo,aux3.c_str());
    for (int i=0;i<20;i++){
        nombre[i] = toupper(nombre[i]);
        tipo[i] = toupper(tipo[i]);
    }
    string mensaje;
    mensaje=compararMed(nombre,tipo,codigo,contadorMedicamento);
    if(buscarIdTipo(tipo,contadorTipoDeMedicamento)==0){
        mensaje="No existe ese tipo de medicamento";
    }
    std::ostringstream ostr;
    if(mensaje.size()==0){
        *contMed=*contMed+1;
        *dat=*contMed;
        sumarContadorDeMed(*contMed);
        m[*contMed].id=*contMed;
        strcpy(m[*contMed].codigo, codigo);
        strcpy(m[*contMed].nombre, nombre);
        strcpy(m[*contMed].droga, droga);
        m[*contMed].activo=true;
        m[*contMed].tdm.id=buscarIdTipo(tipo,contadorTipoDeMedicamento);
        strcpy(m[*contMed].tdm.tipo, tipo);
        m[*contMed].tdm.activo=buscarActivoTipo(tipo,contadorTipoDeMedicamento);
        archivo.write((char *)&m[*contMed],sizeof(Medicamento));
        mensaje="\nEl medicamento se ha creado correctamente. Su id es:";
        //mensaje+=m[contadorMedicamento].id+'0';
        ostr << m[*contMed].id;
        std::string identificador = ostr.str();
        mensaje+=identificador;
    }
    archivo.close();
    return mensaje;
}

void Tip(int contadorTipoDeMedicamento){
    leerTipoDeMedicamento(contadorTipoDeMedicamento);
    for(int i=0;i<contadorTipoDeMedicamento;i++){
        cout<<"\n-----------Tipo de Medicamento-----------: "<<endl;
        cout<<"\nId: "<<t[i].id<<endl;
        cout<<"\nTipo: "<<t[i].tipo<<endl;
        cout<<"\nActivo: "<<t[i].activo<<endl;
    }
}

void Med(int contadorMedicamento){
    leerMedicamento(contadorMedicamento);
    for(int i=0;i<contadorMedicamento;i++){
        cout<<"\n-----------Medicamento-----------: "<<endl;
        cout<<"\nId: "<<m[i].id<<endl;
        cout<<"\nCodigo: "<<m[i].codigo<<endl;
        cout<<"\nNombre: "<<m[i].nombre<<endl;
        cout<<"\nDroga: "<<m[i].droga<<endl;
        cout<<"\nActivo: "<<m[i].activo<<endl;
        cout<<"\nid del Tipo de Medicamento: "<<m[i].tdm.id<<endl;
        cout<<"\ntipo del Tipo de Medicamento: "<<m[i].tdm.tipo<<endl;
        cout<<"\nactivo del Tipo de Medicamento: "<<m[i].tdm.activo<<endl;
    }
}

void leerContadorDeMed(int *contMed){
    ifstream archivo;
    string texto;

    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\contador1.txt",ios::in);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    int a=0;
    while(!archivo.eof()){
        getline(archivo,texto);
        if(a==0){
            //contadorMedicamento=atoi(texto.c_str());
            *contMed=atoi(texto.c_str());
        }
        a++;
    }
    archivo.close();
}

void leerContadorDeTip(int *contTipo){
    ifstream archivo;
    string texto;

    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\contador2.txt",ios::in);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    int a=0;
    while(!archivo.eof()){
        getline(archivo,texto);
        if(a==0){
            //contadorTipoDeMedicamento=atoi(texto.c_str());
            *contTipo=atoi(texto.c_str());
        }
        a++;
    }
    archivo.close();
}

void sumarContadorDeMed(int contadorMedicamento){
    ofstream archivo;
    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\contador1.txt",ios::out);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    archivo<<contadorMedicamento<<endl;
    archivo.close();
}

void sumarContadorDeTip(int contadorTipoDeMedicamento){
    ofstream archivo;
    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\contador2.txt",ios::out);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    archivo<<contadorTipoDeMedicamento<<endl;
    archivo.close();
}

string compararMed(char nombre[20],char tipo[20],char codigo[20],int contadorMedicamento){
    bool flag;
    bool result;
    for(int i=0;i<contadorMedicamento;i++){
        if((strcmp(m[i].nombre,nombre))==0 && (strcmp(m[i].tdm.tipo,tipo))==0){
            flag=true;
        }
    }
    //VERIFICACION DEL CARACTER VERIFICADOR
    char cadena[3];
    int numero,suma=0;
    for(int j=4;j<9;j++){
        cadena[j-4]=codigo[j];
    }
    numero=atoi(cadena);
    while(numero>0){
        suma=suma+numero%10;
        numero=numero/10;
    }
    numero=suma;
    suma=0;
    while(numero>0){
        suma=suma+numero%10;
        numero=numero/10;
    }
    char n;
    n=suma+'0';
    char verificador=codigo[10];
    string mensaje;
    if(suma==10){
        n=1+'0';
    }
    if(n!=verificador){
        mensaje="\nEl codigo esta mal";
    }
    if(flag==true){
        mensaje+="\nEl medicamento ";
        mensaje+=nombre;
        mensaje+="-";
        mensaje+=tipo;
        mensaje+=" ya existe\npor favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente: ";
    }
    return mensaje;
}

bool compararTipo(char denominacion[20],int contadorTipoDeMedicamento){
    bool flag;
    leerTipoDeMedicamento(contadorTipoDeMedicamento);
    for(int i=0;i<contadorTipoDeMedicamento;i++){
        if(strcmp(t[i].tipo,denominacion)==0){
            flag=true;
        }
    }
    return flag;
}

string modificarMed(char id[20],int contadorMedicamento,int *medAMod){
    string str(id);
    int inicio=0;
    int contador=0;
    char auxiliar[20];
    for(int i=0;i<str.size();i++){
        if(str[i]==':'){
            inicio=i+1;
        }else{
            contador++;
        }
    }
    string aux1=str.substr(inicio,contador);
    strcpy(auxiliar,aux1.c_str());

    int identificador=0;
    identificador=atoi(auxiliar);
    string mensaje;
    if(corroborarIdMed(identificador,contadorMedicamento)==true){
        *medAMod=identificador-1;
        mensaje="\n----------Modificar Medicamento----------: ";
        mensaje+="\nIndique el Nombre o '@' para no modificar este campo: ";
        mensaje+="\nIndique el Codigo o '@' para no modificar este campo: ";
        mensaje+="\nIndique la Droga o '@' para no modificar este campo: ";
        mensaje+="\nIndique el Tipo de medicamento o '@' para no modificar este campo: ";
        mensaje+="\nIndique el Alta o Baja, valores aceptados: 's' para darlo de alta\n 'n' para darlo debaja '@' para no modificar este campo: ";
    }else{
        mensaje="\nNo existe el medicamento con el id indicado";
    }
    return mensaje;
}

string pedirDatosMed(char informacion[50],int contadorMedicamento,int contadorTipoDeMedicamento,int medAModificar,struct Medicamento *medAux){//FALTA MED A MODIFICAR
    char nombre[20];
    char codigo[20];
    char droga[20];
    char tipo[20];
    char alta;
    string str(informacion);
    int stop1=0;
    int stop2=0;
    int stop3=0;
    int contador1=0;
    int contador2=0;
    int contador3=0;
    int contador4=0;
    int inicio=1;
    for(int i=0;i<str.size();i++){
        if(str[i]==','){
            inicio++;
        }
        switch(inicio){
            case 1:{
                contador1++;
            break;
            }
            case 2:{
                if(str[i]==','){
                    stop1=i+1;
                }else{
                    contador2++;
                }
            break;
            }
            case 3:{
                if(str[i]==','){
                    stop2=i+1;
                }else{
                    contador3++;
                }
            break;
            }
            case 4:{
                if(str[i]==','){
                    stop3=i+1;
                }else{
                    contador4++;
                }
            break;
            }
            case 5:{
                if(str[i]!=','){
                   alta=str[i];
                }
            break;
            }
        }
    }
    string aux1=str.substr(0,contador1);
    string aux2=str.substr(stop1,contador2);
    string aux3=str.substr(stop2,contador3);
    string aux4=str.substr(stop3,contador4);
    strcpy(nombre,aux1.c_str());
    strcpy(codigo,aux2.c_str());
    strcpy(droga,aux3.c_str());
    strcpy(tipo,aux4.c_str());
    for (int i=0;i<20;i++){
        nombre[i] = toupper(nombre[i]);
        tipo[i] = toupper(tipo[i]);
    }
    medAux->id=m[medAModificar].id;
    strcpy(medAux->codigo,m[medAModificar].codigo);
    strcpy(medAux->nombre,m[medAModificar].nombre);
    strcpy(medAux->droga,m[medAModificar].droga);
    medAux->activo=m[medAModificar].activo;
    medAux->tdm.id=m[medAModificar].tdm.id;
    strcpy(medAux->tdm.tipo,m[medAModificar].tdm.tipo);
    medAux->tdm.activo=m[medAModificar].tdm.activo;

    string mensaje;
    if(strcmp(codigo,"@")!=0){
        mensaje=compararMed("A","A",codigo,contadorMedicamento);
        mensaje+=compararCodigo(codigo,contadorMedicamento);
    }
    if(strcmp(nombre,"@")!=0 && strcmp(tipo,"@")!=0 && strcmp(codigo,"@")!=0){
        mensaje+=compararMed(nombre,tipo,codigo,contadorMedicamento);
    }
    if(strcmp(nombre,"@")!=0 && strcmp(tipo,"@")!=0 && strcmp(codigo,"@")==0){
        mensaje+=compararMed(nombre,tipo,"SLH-11223-9",contadorMedicamento);
    }
    if(strcmp(nombre,"@")!=0 && strcmp(tipo,"@")==0){
        mensaje+=compararMed(nombre,m[medAModificar].tdm.tipo,"SLH-11223-9",contadorMedicamento);
    }
    if(strcmp(nombre,"@")==0 && strcmp(tipo,"@")!=0){
        mensaje+=compararMed(m[medAModificar].nombre,tipo,"SLH-11223-9",contadorMedicamento);
    }
    if(strcmp(tipo,"@")!=0 && buscarIdTipo(tipo,contadorTipoDeMedicamento)==0){
        mensaje+="No existe ese tipo de medicamento";
    }
    if(mensaje.size()==0){
        if(strcmp(nombre,"@")!=0){
            strcpy(medAux->nombre, nombre);
        }
        if(strcmp(codigo,"@")!=0){
            strcpy(medAux->codigo, codigo);
        }
        if(strcmp(droga,"@")!=0){
            strcpy(medAux->droga, droga);
        }
        if(strcmp(tipo,"@")!=0){
            medAux->tdm.id=buscarIdTipo(tipo,contadorTipoDeMedicamento);
            strcpy(medAux->tdm.tipo, tipo);
            medAux->tdm.activo=buscarActivoTipo(tipo,contadorTipoDeMedicamento);
        }
        if(alta!='@'){
            if(alta=='s'){
                medAux->activo=true;
            }
            if(alta=='n'){
                medAux->activo=false;
            }
        }
        mensaje="Desea guardar los cambios? s/n: ";
    }else{
        mensaje="\nSe ha producido algun error en alguno de los datos";
    }

    return mensaje;
}

string guardoMed(int medAModificar,struct Medicamento mAux){//FALTA MED A MODIFICAR
    strcpy(m[medAModificar].nombre,mAux.nombre);
    strcpy(m[medAModificar].codigo,mAux.codigo);
    strcpy(m[medAModificar].droga,mAux.droga);
    m[medAModificar].tdm.id=mAux.tdm.id;
    strcpy(m[medAModificar].tdm.tipo,mAux.tdm.tipo);
    m[medAModificar].tdm.activo=mAux.tdm.activo;
    m[medAModificar].activo=mAux.activo;
    reescribirArchivoMed(medAModificar);
    string mensaje;
    std::ostringstream ostr;
    mensaje="\nId: ";
    ostr << m[medAModificar].id;
    std::string identificador = ostr.str();
    mensaje+=identificador;
    //mensaje+=m[medAModificar].id+'0';
    mensaje+="\nNombre: ";
    mensaje+=m[medAModificar].nombre;
    mensaje+="\nCodigo: ";
    mensaje+=m[medAModificar].codigo;
    mensaje+="\nDroga: ";
    mensaje+=m[medAModificar].droga;
    mensaje+="\nTipo: ";
    mensaje+=m[medAModificar].tdm.tipo;
    mensaje+="\nActivo: ";
    mensaje+=m[medAModificar].activo+'0';
    return mensaje;
}

string modificarTipo(char id[20],int contadorTipoDeMedicamento,int *tipAMod){
    char tipo[20];
    char activo[20];
    string str(id);
    int inicio=0;
    int contador=0;
    char auxiliar[20];
    for(int i=0;i<str.size();i++){
        if(str[i]==':'){
            inicio=i+1;
        }else{
            contador++;
        }
    }
    string aux1=str.substr(inicio,contador);
    strcpy(auxiliar,aux1.c_str());

    int identificador=0;
    identificador=atoi(auxiliar);
    string mensaje;
    if(corroborarIdTipo(identificador,contadorTipoDeMedicamento)==true){
        *tipAMod=identificador-1;
        mensaje="\n----------Modificar Tipo de Medicamento----------: ";
        mensaje+="\nIndique la Denominacion/Tipo o '@' para no modificar este campo: ";
        mensaje+="\nIndique que desea hacer\nValores aceptados: 's' para darlo de alta, 'n' para darlo debaja\n '@' Para no modificar este campo: ";
    }else{
        mensaje="\nNo existe el tipo de medicamento con el id indicado";
    }
    return mensaje;
}

string pedirDatosTipo(char informacion[50],int contadorTipoDeMedicamento,int tipoAModificar, struct TipoDeMedicamento *tipAux){//FALTA TIPO A MODIFICAR
    char tipo[20];
    char alta;
    string str(informacion);
    int contador1=0;
    int inicio=1;
    for(int i=0;i<str.size();i++){
        if(str[i]==','){
            inicio++;
        }
        switch(inicio){
            case 1:{
                contador1++;
            break;
            }
            case 2:{
                if(str[i]!=','){
                   alta=str[i];
                }
            break;
            }
        }
    }
    string aux1=str.substr(0,contador1);
    strcpy(tipo,aux1.c_str());
    for (int i=0;i<20;i++){
        tipo[i] = toupper(tipo[i]);
    }
    string mensaje;
    tipAux->id=t[tipoAModificar].id;
    strcpy(tipAux->tipo,t[tipoAModificar].tipo);
    tipAux->activo=t[tipoAModificar].activo;

    if(compararTipo(tipo,contadorTipoDeMedicamento)==false){
        if(alta!='@'){
            if(alta=='s'){
                tipAux->activo=true;
            }
            if(alta=='n'){
                tipAux->activo=false;
            }
        }
        if(strcmp(tipo,"@")!=0){
            strcpy(tipAux->tipo, tipo);
        }
        mensaje="Desea guardar los cambios? s/n: ";
    }else{
        mensaje="\nEl tipo de medicamento especificado ya existe.";
    }
    return mensaje;
}

string guardoTipo(int tipoAModificar, struct TipoDeMedicamento tAux){//FALTA TIPO A MODIFICAR
    strcpy(t[tipoAModificar].tipo,tAux.tipo);
    t[tipoAModificar].activo=tAux.activo;
    reescribirArchivoTipo(tipoAModificar);
    string mensaje;
    std::ostringstream ostr;
    mensaje="\nId: ";
    ostr << t[tipoAModificar].id;
    std::string identificador = ostr.str();
    mensaje+=identificador;
    //mensaje+=t[tipoAModificar].id+'0';
    mensaje+="\nTipo: ";
    mensaje+=t[tipoAModificar].tipo;
    mensaje+="\nActivo: ";
    mensaje+=t[tipoAModificar].activo+'0';
    return mensaje;
}

bool corroborarIdMed(int id,int contadorMedicamento){
    leerMedicamento(contadorMedicamento);
    bool flag;
    for(int i=0;i<contadorMedicamento;i++){
        if(m[i].id==id){
            flag=true;
        }
    }
    return flag;
}

bool corroborarIdTipo(int id,int contadorTipoDeMedicamento){
    leerTipoDeMedicamento(contadorTipoDeMedicamento);
    bool flag;
    for(int i=0;i<contadorTipoDeMedicamento;i++){
        if(t[i].id==id){
            flag=true;
        }
    }
    return flag;
}

int buscarIdTipo(char tipo[20],int contadorTipoDeMedicamento){
    leerTipoDeMedicamento(contadorTipoDeMedicamento);
    int id=0;
    for(int i=0;i<contadorTipoDeMedicamento;i++){
        if(strcmp(t[i].tipo,tipo)==0){
            id=t[i].id;
        }
    }
    return id;
}

bool buscarActivoTipo(char tipo[20],int contadorTipoDeMedicamento){
    leerTipoDeMedicamento(contadorTipoDeMedicamento);
    bool activo;
    for(int i=0;i<contadorTipoDeMedicamento;i++){
        if(strcmp(t[i].tipo,tipo)==0){
            activo=t[i].activo;
        }
    }
    return activo;
}

string compararCodigo(char codigo[20],int contadorMedicamento){
    leerMedicamento(contadorMedicamento);
    string existe;
    for(int i=0;i<contadorMedicamento;i++){
        if(strcmp(m[i].codigo,codigo)==0){
            existe="\nYa existe un medicamento con ese codigo";
        }
    }
    return existe;
}

void reescribirArchivoMed(int medAModificar){//FALTA MED A MODIFICAR
    FILE *arch;
    arch=fopen("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\Medicamento.dat","r+b");
    if (arch==NULL){
        exit(1);
    }
    Medicamento med;
    fread(&med, sizeof(Medicamento), 1, arch);
    int i=0;
    while(!feof(arch))
    {
        if (m[i].id==medAModificar+1)
        {
           int pos=ftell(arch)-sizeof(Medicamento);
           fseek(arch,pos,SEEK_SET);
           fwrite(&m[i], sizeof(Medicamento), 1, arch);
           break;
        }
        fread(&m[i], sizeof(Medicamento), 1, arch);
        i++;
    }
    fclose(arch);
}

void reescribirArchivoTipo(int tipoAModificar){//FALTA TIPO A MODIFICAR
    FILE *arch;
    arch=fopen("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\TipoDeMedicamento.dat","r+b");
    if (arch==NULL){
        exit(1);
    }
    TipoDeMedicamento tp;
    fread(&tp, sizeof(TipoDeMedicamento), 1, arch);
    int i=0;
    while(!feof(arch))
    {
        if (t[i].id==tipoAModificar+1)
        {
           int pos=ftell(arch)-sizeof(TipoDeMedicamento);
           fseek(arch,pos,SEEK_SET);
           fwrite(&t[i], sizeof(TipoDeMedicamento), 1, arch);
           break;
        }
        fread(&t[i], sizeof(TipoDeMedicamento), 1, arch);
        i++;
    }
    fclose(arch);
}

string administrarMed(char datos[20],int contadorMedicamento){
    int inicio=1;
    string str(datos);
    char nombre[20];
    char tipo[20];
    int contar=0;
    int con2=0;
    int con3=0;
    int stop1=0;
    for(int i=0;i<str.size();i++){
        if(str[i]==','){
            inicio=2;
        }
        switch(inicio){
            case 1:{
                con2++;
            break;
            }
            case 2:{
                if(str[i]!=','){
                    con3++;
                }else{
                    stop1=i+1;
                }
            break;
            }
        }
    }
    string aux1=str.substr(0,con2);
    string aux2=str.substr(stop1,con3);
    strcpy(nombre,aux1.c_str());
    strcpy(tipo,aux2.c_str());
    leerMedicamento(contadorMedicamento);
    string mensaje;
    std::ostringstream ostr;

    if(strcmp(nombre,"@")!=0 && strcmp(tipo,"@")!=0){
        for(int i=0;i<contadorMedicamento;i++){
            if((strcmp(m[i].nombre,nombre)==0 && strcmp(m[i].tdm.tipo,tipo)==0) || (con2==1 && m[i].nombre[0]==nombre[0] && strcmp(m[i].tdm.tipo,tipo)==0) || (con3==1 && strcmp(m[i].nombre,nombre)==0 && m[i].tdm.tipo[0]==tipo[0]) || (con2==1 && m[i].nombre[0]==nombre[0] && con3==1 && m[i].tdm.tipo[0]==tipo[0])){
                mensaje+="\n-----------Medicamento-----------: ";
                mensaje+="\nId: ";
                //mensaje+=m[i].id+'0';
                ostr << m[i].id;
                std::string identificador = ostr.str();
                mensaje+=identificador;
                mensaje+="\nCodigo: ";
                mensaje+=m[i].codigo;
                mensaje+="\nNombre: ";
                mensaje+=m[i].nombre;
                mensaje+="\nDroga: ";
                mensaje+=m[i].droga;
                mensaje+="\ntipo del Tipo de Medicamento: ";
                mensaje+=m[i].tdm.tipo;
                mensaje+=".";
                contar++;
                ostr.str("");
                ostr.clear();
            }
        }
    }
    if(strcmp(nombre,"@")!=0 && strcmp(tipo,"@")==0){
        for(int i=0;i<contadorMedicamento;i++){
            if((strcmp(m[i].nombre,nombre)==0) || (con2==1 && m[i].nombre[0]==nombre[0])){
                mensaje+="\n-----------Medicamento-----------: ";
                mensaje+="\nId: ";
                //mensaje+=m[i].id+'0';
                ostr << m[i].id;
                std::string identificador = ostr.str();
                mensaje+=identificador;
                mensaje+="\nCodigo: ";
                mensaje+=m[i].codigo;
                mensaje+="\nNombre: ";
                mensaje+=m[i].nombre;
                mensaje+="\nDroga: ";
                mensaje+=m[i].droga;
                mensaje+="\ntipo del Tipo de Medicamento: ";
                mensaje+=m[i].tdm.tipo;
                mensaje+=".";
                contar++;
                ostr.str("");
                ostr.clear();
            }
        }
    }
    if(strcmp(nombre,"@")==0 && strcmp(tipo,"@")!=0){
        for(int i=0;i<contadorMedicamento;i++){
            if((strcmp(m[i].tdm.tipo,tipo)==0) || (con3==1 && m[i].tdm.tipo[0]==tipo[0])){
                mensaje+="\n-----------Medicamento-----------: ";
                mensaje+="\nId: ";
                //mensaje+=m[i].id+'0';
                ostr << m[i].id;
                std::string identificador = ostr.str();
                mensaje+=identificador;
                mensaje+="\nCodigo: ";
                mensaje+=m[i].codigo;
                mensaje+="\nNombre: ";
                mensaje+=m[i].nombre;
                mensaje+="\nDroga: ";
                mensaje+=m[i].droga;
                mensaje+="\ntipo del Tipo de Medicamento: ";
                mensaje+=m[i].tdm.tipo;
                mensaje+=".";
                contar++;
                ostr.str("");
                ostr.clear();
            }
        }
    }
    if(strcmp(nombre,"@")==0 && strcmp(tipo,"@")==0){
        for(int i=0;i<contadorMedicamento;i++){
            mensaje+="\n-----------Medicamento-----------: ";
            mensaje+="\nId: ";
            //mensaje+=m[i].id+'0';
            ostr << m[i].id;
            std::string identificador = ostr.str();
            mensaje+=identificador;
            mensaje+="\nCodigo: ";
            mensaje+=m[i].codigo;
            mensaje+="\nNombre: ";
            mensaje+=m[i].nombre;
            mensaje+="\nDroga: ";
            mensaje+=m[i].droga;
            mensaje+="\ntipo del Tipo de Medicamento: ";
            mensaje+=m[i].tdm.tipo;
            mensaje+=".";
            contar++;
            ostr.str("");
            ostr.clear();
        }
    }
    if(contar==0){
        mensaje="No se encontro ningun resultado para los criterios seleccionados.\nPresione ENTER para continuar";
    }
    return mensaje;
}

string administrarTipo(char datos[20],int contadorTipoDeMedicamento){
    char activo;
    int inicio=1;
    string str(datos);
    char tipo[20];
    bool flag;
    int con1=0;
    int con2=0;
    for(int i=0;i<str.size();i++){
        if(str[i]==','){
            inicio=2;
        }
        switch(inicio){
            case 1:{
                con2++;
            break;
            }
            case 2:{
                if(str[i]!=',' && (str[i]=='s' || str[i]=='n' || str[i]=='@')){
                    activo=str[i];
                }
            break;
            }
        }
    }
    string aux1=str.substr(0,con2);
    strcpy(tipo,aux1.c_str());

    if(activo=='s'){
        flag=true;
    }
    if(activo=='n'){
        flag=false;
    }
    leerTipoDeMedicamento(contadorTipoDeMedicamento);
    string mensaje;
    std::ostringstream ostr;

    if(strcmp(tipo,"@")!=0 && activo!='@'){
        for(int i=0;i<contadorTipoDeMedicamento;i++){
            if((strcmp(t[i].tipo,tipo)==0 && t[i].activo==flag) || (con2==1 && t[i].tipo[0]==tipo[0] && t[i].activo==flag)){
                mensaje+="\n-----------Tipo de Medicamento-----------: ";
                mensaje+="\nId: ";
                //mensaje+=t[i].id+'0';
                ostr << t[i].id;
                std::string identificador = ostr.str();
                mensaje+=identificador;
                mensaje+="\nTipo: ";
                mensaje+=t[i].tipo;
                mensaje+="\nActivo: ";
                mensaje+=t[i].activo+'0';
                mensaje+=".";
                con1++;
                ostr.str("");
                ostr.clear();
            }
        }
    }
    if(strcmp(tipo,"@")!=0 && activo=='@'){
        for(int i=0;i<contadorTipoDeMedicamento;i++){
            if((strcmp(t[i].tipo,tipo)==0) || (con2==1 && t[i].tipo[0]==tipo[0])){
                mensaje+="\n-----------Tipo de Medicamento-----------: ";
                mensaje+="\nId: ";
                //mensaje+=t[i].id+'0';
                ostr << t[i].id;
                std::string identificador = ostr.str();
                mensaje+=identificador;
                mensaje+="\nTipo: ";
                mensaje+=t[i].tipo;
                mensaje+="\nActivo: ";
                mensaje+=t[i].activo+'0';
                mensaje+=".";
                con1++;
                ostr.str("");
                ostr.clear();
            }
        }
    }
    if(strcmp(tipo,"@")==0 && activo!='@'){
        for(int i=0;i<contadorTipoDeMedicamento;i++){
            if(t[i].activo==flag){
                mensaje+="\n-----------Tipo de Medicamento-----------: ";
                mensaje+="\nId: ";
                //mensaje+=t[i].id+'0';
                ostr << t[i].id;
                std::string identificador = ostr.str();
                mensaje+=identificador;
                mensaje+="\nTipo: ";
                mensaje+=t[i].tipo;
                mensaje+="\nActivo: ";
                mensaje+=t[i].activo+'0';
                mensaje+=".";
                con1++;
                ostr.str("");
                ostr.clear();
            }
        }
    }
    if(strcmp(tipo,"@")==0 && activo=='@'){
        for(int i=0;i<contadorTipoDeMedicamento;i++){
            mensaje+="\n-----------Tipo de Medicamento-----------: ";
            mensaje+="\nId: ";
            //mensaje+=t[i].id+'0';
            ostr << t[i].id;
            std::string identificador = ostr.str();
            mensaje+=identificador;
            mensaje+="\nTipo: ";
            mensaje+=t[i].tipo;
            mensaje+="\nActivo: ";
            mensaje+=t[i].activo+'0';
            mensaje+=".";
            con1++;
            ostr.str("");
            ostr.clear();
        }
    }
    if(con1==0){
        mensaje="No se encontro ningun resultado para los criterios seleccionados.\nPresione ENTER para continuar";
    }
    return mensaje;
}

void escribirArchivoLogInicio(int puertoEscucha){
    ofstream archivo;
    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\server.log.txt",ios::app);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    char fechaHora[20];
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    sprintf(fechaHora,"%d-%d-%d_%d:%d: ",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min);
    archivo<<fechaHora<<"======================================"<<endl;
    archivo<<fechaHora<<"===========Inicia Servidor============"<<endl;
    archivo<<fechaHora<<"======================================"<<endl;
    archivo<<fechaHora<<"Socket creado. Puerto de escucha:"<<puertoEscucha<<"."<<endl;
    archivo.close();
}

void escribirArchivoLogIngreso(string usuarioActual){
    ofstream archivo;
    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\server.log.txt",ios::app);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    char fechaHora[20];
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    sprintf(fechaHora,"%d-%d-%d_%d:%d: ",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min);
    archivo<<fechaHora<<"======================================"<<endl;
    archivo<<fechaHora<<"Usuario: "<<usuarioActual<<" ha ingresado"<<endl;
    archivo<<fechaHora<<"======================================"<<endl;
    archivo.close();
}

void escribirArchivoLogCierre(string usuarioActual){
    ofstream archivo;
    archivo.open("C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\server.log.txt",ios::app);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    char fechaHora[20];
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    sprintf(fechaHora,"%d-%d-%d_%d:%d: ",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min);
    archivo<<fechaHora<<"======================================"<<endl;
    archivo<<fechaHora<<"Usuario: "<<usuarioActual<<" ha salido"<<endl;
    archivo<<fechaHora<<"======================================."<<endl;
    archivo.close();
}

void escribirArchivoUserInicia(string usuarioActual){
    ofstream archivo;
    string extension=".txt";
    string direccion="C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\";
    direccion.append(usuarioActual);
    direccion.append(extension);
    archivo.open(direccion.c_str(),ios::app);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    char fechaHora[20];
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    sprintf(fechaHora,"%d-%d-%d_%d:%d: ",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min);
    archivo<<fechaHora<<"===========Inicia Sesion===========."<<endl;
    archivo.close();
}

void escribirArchivoUserCierra(string usuarioActual){
    ofstream archivo;
    string extension=".txt";
    string direccion="C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\";
    direccion.append(usuarioActual);
    direccion.append(extension);
    archivo.open(direccion.c_str(),ios::app);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    char fechaHora[20];
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    sprintf(fechaHora,"%d-%d-%d_%d:%d: ",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min);
    archivo<<fechaHora<<"===========Cierra Sesion===========."<<endl;
    archivo.close();
}

void escribirArchivoUserAccion(int id,string accion,string usuarioActual){
    ofstream archivo;
    string extension=".txt";
    string direccion="C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\";
    direccion.append(usuarioActual);
    direccion.append(extension);
    archivo.open(direccion.c_str(),ios::app);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    char fechaHora[20];
    time_t t=time(NULL);
    struct tm tm=*localtime(&t);
    char aux[60];
    strcpy(aux,accion.c_str());
    sprintf(fechaHora,"%d-%d-%d_%d:%d: ",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min);
    archivo<<fechaHora<<" - "<<aux<<" "<<id+1<<"."<<endl;
    archivo.close();
}

string transferirArchivo(string usuarioActual){
    ifstream archivo;
    string extension=".txt";
    string direccion="C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Server\\bin\\Debug\\";
    direccion.append(usuarioActual);
    direccion.append(extension);
    archivo.open(direccion.c_str(),ios::in);
    if(archivo.fail()){
        cout<<"No se pudo abrir el archivo"<<endl;
        exit(1);
    }
    //Intento de enviar el archivo al destino
    /*string cadena;
    string destino="C:\\Users\\wilro_000\\Desktop\\Programas Roger\\Practicas en C++\\Cliente-Servidor\\Cliente\\bin\\Debug\\" + usuarioActual + extension;
    cadena = "copy " + direccion + " " + destino;
    system(cadena.c_str());
    archivo.close();*/

    string texto;
    string mensaje;
    while(!archivo.eof()){
        getline(archivo,texto);
        mensaje+="\n"+texto;
    }
    archivo.close();
    return mensaje;
}

void leerYMostraerDatosBinarios(int contadorTipoDeMedicamento,int contadorMedicamento){
    Tip(contadorTipoDeMedicamento);
    Med(contadorMedicamento);
}

int medio(string datos){
    int inicio=0;
    bool flag;
    int pos1;
    int pos2;
    pos2=datos.size()-2;
    for(int i=0;i<datos.size();i++){
        if(datos[i]=='.'){
            inicio++;
        }
    }
    int posicion[inicio-1];
    int contador=0;
    for(int i=0;i<datos.size();i++){
        if(datos[i]=='.'){
            posicion[contador]=i+1;
            contador++;
        }
    }
    return posicion[(inicio-1)/2];
}

string primeraParte(string datos){
    string aux1=datos.substr(0,medio(datos));
    return aux1;
}

string segundaParte(string datos){
    string aux1=datos.substr(medio(datos),datos.size()-2);
    return aux1;
}
