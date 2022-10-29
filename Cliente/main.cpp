#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <cstring>
#include <ctype.h>
#include <cstdlib>
#include <time.h>

using namespace std;


class Client{
public:
    string datos;
    int eleccion;
    int puertoEscucha=5010;
    int contador=0;

    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];
    clock_t t1;
    clock_t t2;
    float tiempo;
    Client()
    {
        cout<<"Conectando al servidor..."<<endl<<endl;
        WSAStartup(MAKEWORD(2,0), &WSAData);
        server = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_addr.s_addr = inet_addr("192.168.0.16");
        addr.sin_family = AF_INET;
        addr.sin_port = htons(puertoEscucha);
        connect(server, (SOCKADDR *)&addr, sizeof(addr));
    }
    void Enviar()
    {
        int opcion=0;
        t1=clock();
        //MUESTRO LOS DATOS RECIBIDOS Y ENVIO UN MENSAJE
        switch(eleccion){
            case 1:{
                system("cls");
                cout << datos << endl;
                cin>>this->buffer;
                break;
            }
            case 2:{
                system("cls");
                cout << datos << endl;
                cin>>this->buffer;
                break;
            }
            case 3:{
                //system("cls");
                cout << "\nIngrese el ID del Tipo de Medicamento\npara modificar o eliminar en un formato (ID:'id'): " << endl;
                cin>>this->buffer;
                break;
            }
            case 4:{
                system("cls");
                cout << datos << endl;
                cin>>this->buffer;
                break;
            }
            case 5:{
                system("cls");
                cout << datos << endl;
                cin>>this->buffer;
                if(buffer[0]!='s'){
                    eleccion=0;
                }
                break;
            }
            case 6:{
                system("cls");
                cout << datos << endl;
                cin>>this->buffer;
                break;
            }
            case 7:{
                //system("cls");
                cout << "\nIngrese el ID del Medicamento\npara modificar o eliminar en un formato (ID:'id'): " << endl;
                cin>>this->buffer;
                break;
            }
            case 8:{
                system("cls");
                cout << datos << endl;
                cin>>this->buffer;
                break;
            }
            case 9:{
                system("cls");
                cout << datos << endl;
                cin>>this->buffer;
                if(buffer[0]!='s'){
                    eleccion=0;
                }
                break;
            }
            case 10:{
                cout << datos << endl;
                cin>>this->buffer;
                break;
            }
            // ---------------------------------------------------------------------
            case 11:{//Recibo y muestro el mensaje de lista de tipo demasiado larga
                //cout << datos << endl;
                cin>>this->buffer;
                eleccion=12;
                break;
            }
            case 12:{//Recibo la primera parte de la lista de tipo y la muestro
                system("cls");
                cout << "\nTipo de Medicamento:" << endl;
                cout << datos << endl;
                cout << "\nPresione cualquier tecla para continuar" << endl;
                cin>>this->buffer;
                eleccion=2;
                break;
            }
            /*case 12:{//Recibo la segunda parte y la muestro
                cout << datos << endl;
                cout << "Presione cualquier tecla para continuar" << endl;
                cin>>this->buffer;
                eleccion=6;
                break;
            }*/
            case 13:{//Recibo y muestro el mensaje de lista de medicamento demasiado larga
                //cout << datos << endl;
                cin>>this->buffer;
                eleccion=14;
                break;
            }
            case 14:{//Recibo la primera parte de la lista de tipo y la muestro
                system("cls");
                cout << "\nListado de Medicamento:" << endl;
                cout << datos << endl;
                cout << "\nPresione cualquier tecla para continuar" << endl;
                cin>>this->buffer;
                eleccion=6;
                break;
            }
            // ---------------------------------------------------------------------
            case 15:{//Recibo y muestro el mensaje de archivo de usuario demasiado largo
                cout << datos << endl;
                cin>>this->buffer;
                eleccion=16;
                break;
            }
            case 16:{//Recibo la primera parte del archivo y lo muestro
                system("cls");
                cout << "\nArchivo:" << endl;
                cout << datos << endl;
                cout << "\nPresione cualquier tecla para continuar" << endl;
                cin>>this->buffer;
                eleccion=1;
                break;
            }

        }
        if(eleccion==0){
            while(opcion!=6){
                system("PAUSE");
                system("cls");
                cout<<"\n------------------Menu------------------"<<endl;
                cout<<"\n Opcion 1: - Tipo de medicamento"<<endl;
                cout<<"\n Opcion 2: - Medicamento"<<endl;
                cout<<"\n Opcion 3: - Ver registro de actividades"<<endl;
                cout<<"\n Opcion 4: - Cerrar sesion"<<endl;
                cout<<"\n Opcion 5: - Ver tiempo de inactividad\n"<<endl;
                cin>>opcion;
                switch(opcion){
                    case 1:{
                        system("PAUSE");
                        system("cls");
                        cout<<"\n---------Tipo de medicamento---------:"<<endl;
                        cout<<"\nOpcion 1: - Crear"<<endl;
                        cout<<"\nOpcion 2: - Administrar"<<endl;
                        cout<<"\nOpcion 3: - Volver\n"<<endl;
                        cin>>this->buffer;
                    break;
                    }
                    case 2:{
                        system("PAUSE");
                        system("cls");
                        cout<<"\n---------Medicamento---------:"<<endl;
                        cout<<"\nOpcion 3: - Volver"<<endl;
                        cout<<"\nOpcion 4: - Crear"<<endl;
                        cout<<"\nOpcion 5: - Administrar\n"<<endl;
                        cin>>this->buffer;
                    break;
                    }
                    case 3:{
                        system("PAUSE");
                        system("cls");
                        buffer[0]='6';
                        eleccion=1;
                    break;
                    }
                    case 4:{
                        system("PAUSE");
                        system("cls");
                        buffer[0]='7';
                        cout<<"Sesion cerrada"<<endl;
                        system("PAUSE");
                        exit(1);
                    break;
                    }
                    case 5:{
                        system("PAUSE");
                        system("cls");
                        t2=clock()-t1;
                        cout<<float(t2)/CLOCKS_PER_SEC<<endl;
                        tiempo=float(t2)/CLOCKS_PER_SEC;
                        if(tiempo>=120){
                            cout<<"Se ha superado los 2 minutos de inactividad"<<endl;
                            buffer[0]='7';
                        }else{
                            buffer[0]='3';
                        }
                    break;
                    }
                }
                if(buffer[0]!='3'){
                    break;
                }
            }
        }
        send(server, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        //cout << "Mensaje enviado!" << endl;
    }
    void Recibir()
    {
        recv(server, buffer, sizeof(buffer), 0);
        string mensaje(buffer);
        if(mensaje.size()==0){
            cout << "No se encuentra un servidor" << endl;
            system("PAUSE");
            exit(1);
        }
        datos=mensaje;
        //SI LO QUE RECIBO ES UNA SOLICITUD DE DATOS PERMITO ENVIARLOS, SI NO, MUESTRO LOS DATOS Y VOY AL MENU
        if(strcmp(buffer,"El archivo de actividades del usuario es muy largo, se dividira en dos mensajes\nPresione cualquier tecla para continuar")==0){
            eleccion=15;
        }

        switch(eleccion){
            case 1:{
                cout<<buffer<<endl;
                eleccion=0;
            break;
            }
            case 2:{
                cout<<buffer<<endl;
                eleccion=3;
            break;
            }
            case 3:{
                cout<<buffer<<endl;
                eleccion=4;
            break;
            }
            case 4:{
                cout<<buffer<<endl;
                eleccion=5;
            break;
            }
            case 5:{
                cout<<buffer<<endl;
                eleccion=0;
            break;
            }
            case 6:{
                cout<<buffer<<endl;
                eleccion=7;
            break;
            }
            case 7:{
                cout<<buffer<<endl;
                eleccion=8;
            break;
            }
            case 8:{
                cout<<buffer<<endl;
                eleccion=9;
            break;
            }
            case 9:{
                cout<<buffer<<endl;
                eleccion=0;
            break;
            }
        }
        //RECIBO LOS DATOS Y ASIGNO UNA VARIABLE PARA MOSTRARLOS EN EL ENVIAR
        if(strcmp(buffer,"\n\tIngrese la denominacion y si esta activo\n\ten un formato(denominacion,activo):\nDenominacion:\n'c' traera todos los tipos que empiezan con c.\n'jarabe' para traer el tipo jarabe.\n'@' para no aplicar filtro\nActivo:\n's' traera todos los Activos.\n'n' traera todos los Inactivos.\n'@' para no aplicar filtro")==0){
            eleccion=2;
        }
        if(strcmp(buffer,"\nIngrese el nombre comercial y si el tipo de medicamento\n en un formato (nombre,tipo):\nNombre comercial:\n'a' traera todos los medicamentos que empiezan con a.\n'durezol' para traer el medicamento durezol.\n'@' para no aplicar filtro\nTipo de medicamento:\n'c' traera todos los medicamentos de tipo que empiezan con c.\n'jarabe' traera todos los medicamentos de tipo jarabe.\n'@' para no aplicar filtro")==0){
            eleccion=6;
        }
        if(strcmp(buffer,"No se encontro ningun resultado para los criterios seleccionados.\nPresione ENTER para continuar")==0
            || strcmp(buffer,"\nNo existe el tipo de medicamento con el id indicado")==0
            || strcmp(buffer,"\nNo existe el medicamento con el id indicado")==0
            || strcmp(buffer,"\nEl tipo de medicamento especificado ya existe.")==0
            || strcmp(buffer,"\nSe ha producido algun error en alguno de los datos")==0){
            eleccion=0;
        }
        if(strcmp(buffer,"\nIndique el User y Password en un formato (user;password)")==0){
            if(contador>0){
                system("cls");
                cout<<"\nEl user o password ingresado es incorrecto. "<<endl;
            }
            contador++;
            eleccion=10;
        }
        if(strcmp(buffer,"se supero la cantidad maxima de intentos de ingreso")==0){
            system("cls");
            strcpy(buffer,"\nEl user o password ingresado es incorrecto. \nSe supero la cantidad maxima de intentos de ingreso\n\nIndique el User y Password en un formato (user;password)");
            string mensaje1(buffer);
            datos=mensaje1;
            /*cout<<"\nEl user o password ingresado es incorrecto. "<<endl;
            cout<<"\nIndique el User y Password en un formato (user;password)\n\nPara intentar acceder de nuevo ya que "<<endl;*/
            eleccion=10;
            //contador=0;
        }
        if(strcmp(buffer,"\nSe ha aceptado la conexion")==0){
            cout<<buffer<<endl;
            eleccion=0;
            contador=0;
        }
        if(strcmp(buffer,"\n\t\tSesion cerrada\nIndique el User y Password en un formato (user;password)")==0){
            if(contador>0){
                system("cls");
                cout<<"\nEl user o password ingresado es incorrecto. "<<endl;
            }
            contador++;
            eleccion=10;
        }
        if(strcmp(buffer,"\n----------Agregar un Tipo De Medicamento----------:\n\nIndique el tipo:")==0
           || strcmp(buffer,"\n----------Agregar un Medicamento----------:\n\nIngrese un codigo, nombre comercial, droga y el tipo de medicamento\n\ten un formato(codigo,nombre,droga,tipo):")==0){
            eleccion=1;
        }

        if(strcmp(buffer,"La lista de Tipos de Medicamentos es muy larga, se dividira en dos mensajes\nPresione cualquier tecla para continuar")==0){
            eleccion=11;
        }
        if(strcmp(buffer,"La lista de Medicamentos es muy larga, se dividira en dos mensajes\nPresione cualquier tecla para continuar")==0){
            eleccion=13;
        }

        memset(buffer, 0, sizeof(buffer));
    }
    void CerrarSocket()
    {
       closesocket(server);
       WSACleanup();
       cout << "Socket cerrado." << endl << endl;
    }
};

int main()
{
    Client *Cliente = new Client();
    while(true)
    {
        Cliente->Recibir();
        Cliente->Enviar();
    }
}
