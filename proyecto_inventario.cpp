#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include <exception>

using namespace std;

class Cliente
{
private:
    string nombre;
    int id;
    int telefono;

    int generarId()
    {
        return rand() % 1000 + 1;
    }

public:
    Cliente(const string &nombre, int telefono) : nombre(nombre), telefono(telefono), id(generarId()) {}

    int getId() const { return id; }
    string getNombre() const { return nombre; }
    int getTelefono() const { return telefono; }
};

class ExcepcionProducto : public exception
{
public:
    const char *what() const noexcept override
    {
        return "Error relacionado con un producto.";
    }
};

class ExcepcionStockInsuficiente : public ExcepcionProducto
{
public:
    const char *what() const noexcept override
    {
        return "Stock insuficiente para la venta.";
    }
};

class ExcepcionUsuario : public exception
{
public:
    const char *what() const noexcept override
    {
        return "Error relacionado con el usuario.";
    }
};

class Usuario
{
protected:
    string username;
    string password;

public:
    Usuario(const string &username, const string &password) : username(username), password(password) {}

    virtual bool autenticar(const string &input_username, const string &input_password) const
    {
        return username == input_username && password == input_password;
    }

    virtual ~Usuario() = default;
};

class Producto
{
protected:
    string nombre;
    int codigo;
    float precio;
    int cantidad;
    string tipo;

    int generarCodigo()
    {
        return rand() % 9000 + 1000;
    }

public:
    Producto(const string &nombre, float precio, int cantidad, const string &tipo)
        : nombre(nombre), precio(precio), cantidad(cantidad), tipo(tipo), codigo(generarCodigo()) {}

    virtual void vender(int cantidadAVender)
    {
        if (cantidadAVender > cantidad)
        {
            throw ExcepcionStockInsuficiente();
        }
        cantidad -= cantidadAVender;
        cout << "Venta realizada con éxito." << endl;
    }

    virtual ~Producto() = default;

    virtual void mostrarDetalles() const
    {
        cout << "Producto: " << nombre << ", Código: " << codigo << ", Precio: " << precio << ", Cantidad: " << cantidad << ", Tipo: " << tipo << endl;
    }

    int getCodigo() const { return codigo; }
    string getNombre() const { return nombre; }
    float getPrecio() const { return precio; }
    int getCantidad() const { return cantidad; }
    string getTipo() const { return tipo; }
};

class ProductoOferta : public Producto
{
private:
    float descuento;

public:
    ProductoOferta(const string &nombre, float precio, int cantidad, const string &tipo, float descuento)
        : Producto(nombre, precio, cantidad, tipo), descuento(descuento) {}

    void vender(int cantidadAVender) override
    {
        Producto::vender(cantidadAVender);
        cout << "Se aplicó un descuento del " << descuento * 100 << "% al precio." << endl;
    }

    float getPrecioConDescuento() const
    {
        return precio * (1 - descuento);
    }

    void mostrarDetalles() const override
    {
        Producto::mostrarDetalles();
        cout << "Precio con descuento: " << getPrecioConDescuento() << endl;
    }
};

class Tienda
{
private:
    vector<Cliente> clientes;
    map<string, vector<shared_ptr<Producto>>> inventario;
    vector<pair<int, int>> ventas;
    Usuario usuario;
    vector<shared_ptr<Producto>> carrito;
    vector<pair<Cliente, vector<pair<int, int>>>> ventasPorCliente; // Vector de ventas por cliente

public:
    Tienda() : usuario("", "") {}

    void registrarUsuario()
    {
        string nuevoUsuario, nuevaContrasena;
        cout << "Ingrese un nombre de usuario: ";
        getline(cin >> ws, nuevoUsuario);
        cout << "Ingrese una contraseña: ";
        getline(cin >> ws, nuevaContrasena);
        usuario = Usuario(nuevoUsuario, nuevaContrasena);
        cout << "Usuario registrado con éxito." << endl;
    }

    bool login()
    {
        string username, password;
        cout << "Ingrese su usuario: ";
        getline(cin >> ws, username);
        cout << "Ingrese su contraseña: ";
        getline(cin >> ws, password);
        return usuario.autenticar(username, password);
    }

    void agregarProductoOferta()
    {
        inventario["Tecnología"].push_back(make_shared<ProductoOferta>("TV Samsung", 2199, 10, "Tecnología", 0.3));
        inventario["Muebles"].push_back(make_shared<ProductoOferta>("Colchón Paraiso", 399, 5, "Muebles", 0.3));
        inventario["Belleza y Salud"].push_back(make_shared<ProductoOferta>("Gel Cerave", 63.50, 20, "Belleza y Salud", 0.3));
        inventario["Accesorios de Moda"].push_back(make_shared<ProductoOferta>("Anillo Pandora", 1210, 7, "Accesorios de Moda", 0.3));
    }

    void crearInventario()
    {
        string nombre;
        float precio;
        int cantidad;
        int opcion;
        string tipo;

        cout << "Ingrese el nombre del producto: ";
        getline(cin >> ws, nombre);
        cout << "Ingrese el precio del producto: ";
        cin >> precio;
        cout << "Ingrese la cantidad del producto: ";
        cin >> cantidad;

        cout << "Seleccione la categoría del producto: " << endl;
        cout << "1. Tecnología" << endl;
        cout << "2. Muebles" << endl;
        cout << "3. Belleza y Salud" << endl;
        cout << "4. Accesorios de Moda" << endl;
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            tipo = "Tecnología";
            break;
        case 2:
            tipo = "Muebles";
            break;
        case 3:
            tipo = "Belleza y Salud";
            break;
        case 4:
            tipo = "Accesorios de Moda";
            break;
        default:
            cout << "Opción no válida. Se asignará categoría 'Otros'." << endl;
            tipo = "Otros";
            break;
        }

        inventario[tipo].push_back(make_shared<Producto>(nombre, precio, cantidad, tipo));
        cout << "Producto creado con éxito. Código: " << inventario[tipo].back()->getCodigo() << endl;
    }

    void verSeccion(const string &seccion) const
    {
        cout << "\nProductos en la sección: " << seccion << endl;
        if (inventario.find(seccion) != inventario.end())
        {
            for (const auto &producto : inventario.at(seccion))
            {
                producto->mostrarDetalles();
            }
        }
        else
        {
            cout << "No existen productos en esta sección." << endl;
        }
    }

    void agregarAlCarrito()
    {
        int codigoProducto;
        cout << "Ingrese el código del producto a agregar al carrito: ";
        cin >> codigoProducto;

        bool encontrado = false;
        for (auto &seccion : inventario)
        {
            for (auto &producto : seccion.second)
            {
                if (producto->getCodigo() == codigoProducto)
                {
                    carrito.push_back(producto);
                    cout << "Producto agregado al carrito." << endl;
                    encontrado = true;
                    break;
                }
            }
            if (encontrado)
                break;
        }

        if (!encontrado)
        {
            cout << "Producto no encontrado." << endl;
        }
    }

    void verCarrito() const
    {
        if (carrito.empty())
        {
            cout << "El carrito está vacío." << endl;
            return;
        }
        cout << "Carrito de compras:" << endl;
        float total = 0;
        for (const auto &producto : carrito)
        {
            producto->mostrarDetalles();
            total += producto->getPrecio();
        }
        cout << "Total: " << total << endl;
    }

    void crearCliente()
    {
        string nombre;
        int telefono;
        cout << "Ingrese el nombre del cliente: ";
        getline(cin >> ws, nombre);
        cout << "Ingrese el teléfono del cliente: ";
        cin >> telefono;
        clientes.push_back(Cliente(nombre, telefono));
        cout << "Cliente creado con éxito. ID asignado: " << clientes.back().getId() << endl;
    }

    void verClientes() const
    {
        if (clientes.empty())
        {
            cout << "No hay clientes registrados." << endl;
            return;
        }
        for (const auto &cliente : clientes)
        {
            cout << "Cliente: " << cliente.getNombre() << ", ID: " << cliente.getId() << ", Teléfono: " << cliente.getTelefono() << endl;
        }
    }

    void verInventario() const
    {
        if (inventario.empty())
        {
            cout << "El inventario está vacío." << endl;
            return;
        }
        for (const auto &seccion : inventario)
        {
            cout << "\nSección: " << seccion.first << endl;
            for (const auto &producto : seccion.second)
            {
                producto->mostrarDetalles();
            }
        }
    }

    void venderProducto()
    {
        int codigoProducto, cantidad, clienteId;
        cout << "Ingrese el ID del cliente: ";
        cin >> clienteId;

        Cliente *clienteVendido = nullptr;
        for (auto &cliente : clientes)
        {
            if (cliente.getId() == clienteId)
            {
                clienteVendido = &cliente;
                break;
            }
        }

        if (!clienteVendido)
        {
            cout << "Cliente no encontrado." << endl;
            return;
        }

        vector<pair<int, int>> productosVendidos;
        bool continuar = true;
        while (continuar)
        {
            cout << "Ingrese el código del producto a vender: ";
            cin >> codigoProducto;

            bool encontrado = false;
            for (auto &seccion : inventario)
            {
                for (auto &producto : seccion.second)
                {
                    if (producto->getCodigo() == codigoProducto)
                    {
                        cout << "Ingrese la cantidad a vender: ";
                        cin >> cantidad;
                        producto->vender(cantidad);
                        productosVendidos.push_back({codigoProducto, cantidad});
                        encontrado = true;
                        break;
                    }
                }
                if (encontrado)
                    break;
            }

            char masProductos;
            cout << "¿Desea agregar más productos a la venta? (s/n): ";
            cin >> masProductos;
            if (masProductos != 's')
            {
                continuar = false;
            }
        }

        ventasPorCliente.push_back({*clienteVendido, productosVendidos});
        cout << "Venta realizada para el cliente: " << clienteVendido->getNombre() << endl;
    }

    void generarBoleta() const
    {
        cout << "Boleta digital generada con éxito." << endl;
        if (ventasPorCliente.empty())
        {
            cout << "No hay ventas registradas." << endl;
            return;
        }

        for (const auto &venta : ventasPorCliente)
        {
            const Cliente &cliente = venta.first;
            const vector<pair<int, int>> &productosVendidos = venta.second;

            cout << "\nBoleta para el cliente: " << cliente.getNombre() << endl;
            cout << "ID del cliente: " << cliente.getId() << ", Teléfono: " << cliente.getTelefono() << endl;
            cout << "Productos vendidos:" << endl;

            float total = 0;
            for (const auto &productoVenta : productosVendidos)
            {
                int codigoProducto = productoVenta.first;
                int cantidadVendida = productoVenta.second;

                bool encontrado = false;
                for (auto &seccion : inventario)
                {
                    for (auto &producto : seccion.second)
                    {
                        if (producto->getCodigo() == codigoProducto)
                        {
                            cout << "Producto: " << producto->getNombre()
                                 << ", Cantidad: " << cantidadVendida
                                 << ", Precio unitario: " << producto->getPrecio()
                                 << ", Total: " << producto->getPrecio() * cantidadVendida << endl;
                            total += producto->getPrecio() * cantidadVendida;
                            encontrado = true;
                            break;
                        }
                    }
                    if (encontrado)
                        break;
                }
            }

            cout << "Total a pagar: " << total << endl;
        }
    }

    void mostrarEstadisticas() const
    {
        map<string, int> ventasPorTipo;
        for (const auto &venta : ventas)
        {
            bool encontrado = false;
            for (auto &seccion : inventario)
            {
                for (auto &producto : seccion.second)
                {
                    if (producto->getCodigo() == venta.first)
                    {
                        ventasPorTipo[producto->getTipo()] += venta.second;
                        encontrado = true;
                        break;
                    }
                }
                if (encontrado)
                    break;
            }
        }

        cout << "\nEstadísticas de ventas por tipo de producto:" << endl;
        for (const auto &entry : ventasPorTipo)
        {
            cout << "Tipo de producto: " << entry.first << " - Cantidad vendida: " << entry.second << endl;
        }
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    Tienda tienda;
    tienda.registrarUsuario();

    if (!tienda.login())
    {
        cout << "Error de autenticación." << endl;
        return 1;
    }

    tienda.agregarProductoOferta();

    bool salir = false;
    while (!salir)
    {
        int opcion;
        cout << "\n1. Crear cliente \n2. Ver clientes\n3. Crear inventario\n4. Ver inventario\n5. Agregar al carrito\n6. Ver carrito de compras\n7. Vender producto\n8. Generar boleta\n9. Ver estadísticas\n10. Acceder a secciones de productos\n11. Salir\nIngrese su opción: ";
        cin >> opcion;

        try
        {
            switch (opcion)
            {
            case 1:
                tienda.crearCliente();
                break;
            case 2:
                tienda.verClientes();
                break;
            case 3:
                tienda.crearInventario();
                break;
            case 4:
                tienda.verInventario();
                break;
            case 5:
                tienda.agregarAlCarrito();
                break;
            case 6:
                tienda.verCarrito();
                break;
            case 7:
                tienda.venderProducto();
                break;
            case 8:
                tienda.generarBoleta();
                break;
            case 9:
                tienda.mostrarEstadisticas();
                break;
            case 10:
                cout << "Elija una sección:\n1. Tecnología\n2. Muebles\n3. Belleza y Salud\n4. Accesorios de Moda\nOpción: ";
                int seccion;
                cin >> seccion;
                switch (seccion)
                {
                case 1:
                    tienda.verSeccion("Tecnología");
                    break;
                case 2:
                    tienda.verSeccion("Muebles");
                    break;
                case 3:
                    tienda.verSeccion("Belleza y Salud");
                    break;
                case 4:
                    tienda.verSeccion("Accesorios de Moda");
                    break;
                default:
                    cout << "Opción no válida." << endl;
                    break;
                }
                break;
            case 11:
                cout << "Saliendo del programa..." << endl;
                salir = true;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
            }
        }
        catch (const ExcepcionProducto &e)
        {
            cout << e.what() << endl;
        }
    }

    return 0;
}
