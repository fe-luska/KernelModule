## Linux Backdoor Project
O Linux Backdoor Project é um projeto que possibilita a comunicação entre uma máquina virtual com sistema operacional Linux e outro sistema operacional por meio de um servidor e um cliente. Além disso, o projeto inclui a implementação de um módulo de kernel para o sistema Linux, atuando como um agente backdoor para coletar informações dentro do kernel. O objetivo principal é permitir a troca de dados entre as duas máquinas usando uma conexão TCP/IP e, neste caso específico, os dados coletados são as entradas de teclado do usuário do sistema Ubuntu (Máquina Virtual).

## Autores
Fernando Lucas Vieira Souza
Pedro Grando
Pedro 

## Organização do Repositório
### Diretório "client":
Código-fonte do cliente de rede.
A comunicação ocorre através de uma conexão TCP/IP utilizando Netlink. O cliente acessa os arquivos de log do kernel, nos quais o servidor escreve as teclas pressionadas e os envia. O cliente se conecta a um servidor especificado por um endereço IP e porta.

### Diretório "modules"
Esta pasta contém o código do módulo do kernel.
O modulo utiliza a rotina de interrupção do teclado para capturar o código das teclas pressionadas e as escreve em um aqruivo de log do kernel que é lido pelo servidor em userspace.

### Diretório "Server":
Aqui está o código-fonte do servidor de rede.
O servidor cria um socket e aguarda conexões de clientes. Recebe os dados e escreve nos arquivos de log do kernel que serão transimitidos através do socket.

### Framebuffer
O Framebuffer é a estrutura de dados responsável por armazenar o estado atual da tela do computador. Em usersapce é capaz de armazenar informações como0 cores dos pixeis, disposição e outros atributos importantes para renderização

## Requisitos

- Máquina Virtual Linux (Ubuntu)
- Sistema operacional Windows
- Oracle VirtualBox (ou software de virtualização semelhante)
- GCC (GNU Compiler Collection)

## Configuração da Rede Virtual

1. Crie uma rede virtual associada à VM. Essa rede será uma entidade separada da rede física do sistema hospedeiro.

2. Configure regras de encaminhamento para direcionar o tráfego de entrada para a VM desejada. Essas regras garantem que o tráfego seja manipulado e encaminhado corretamente.

## IP Genérico e Network Address Translation (NAT)

1. As VMs geralmente possuem endereços IP internos válidos somente dentro da rede virtual. Para permitir a comunicação com o ambiente externo, crie um IP genérico (endereço IP público) mapeado para o endereço IP interno da VM.

2. Uma opção comum é utilizar uma rede do tipo Network Address Translation (NAT). Essa escolha simplifica a configuração e reduz a necessidade de endereços IP públicos. Com o NAT, as VMs podem compartilhar um único endereço IP externo, enquanto cada VM possui um endereço IP interno. A tradução de endereço é realizada para permitir a comunicação com o ambiente externo.

   No nosso caso, estamos usando a máquina virtual da Oracle (VirtualBox); você pode seguir o seguinte passo a passo para configurar a rede NAT do nosso socket: [Link para o passo a passo](https://stackoverflow.com/questions/48138413/how-to-connect-through-socket-to-virtual-machine).

## Lidando com o Firewall do Windows

1. Observe que o firewall do Windows pode bloquear o tráfego de rede entrante ou saída, o que pode impactar a comunicação entre a VM e o Windows.

2. Certifique-se de configurar as permissões adequadas no firewall do Windows. Isso pode envolver a abertura de portas específicas ou a criação de regras para permitir o tráfego entre a VM e o Windows. Consulte a documentação oficial do Windows para obter instruções específicas de configuração do firewall.

## Configuração do Servidor

1. Abra a máquina virtual (Ubuntu) e navegue até o diretório que contém o código-fonte do servidor.

2. Utilize o seguinte comando para compilar o código do servidor e gerar o executável. Certifique-se de ter o GCC instalado no sistema:

   ```shell
   make
   ```

   Isso irá compilar o código-fonte do servidor e gerar o executável "server".

3. Após a compilação bem-sucedida, execute o servidor usando o seguinte comando:

   ```shell
   ./server
   ```

   O servidor agora está em execução e aguardando conexões.

## Compilação e In

serção do Módulo de Kernel

1. Navegue até o diretório que contém o código-fonte do módulo de kernel.

2. Abra um terminal e execute o seguinte comando para compilar o módulo de kernel:

   ```shell
   make
   ```

   Isso irá compilar o código-fonte do módulo de kernel e gerar o arquivo objeto de kernel "BackdoorModule.ko".

3. Após a compilação bem-sucedida, insira o módulo de kernel utilizando o seguinte comando:

   ```shell
   sudo insmod BackdoorModule.ko
   ```

   Isso irá carregar o módulo de kernel no sistema Ubuntu.

   Nota: Para remover o módulo de kernel, utilize o comando `sudo rmmod BackdoorModule`.

## Configuração do Cliente

1. Abra um terminal no sistema operacional Windows.

2. Navegue até o diretório que contém o código-fonte do cliente.

3. Utilize o seguinte comando para compilar o cliente:

   ```shell
   make
   ```

   Esse comando utilizará o Makefile fornecido para compilar o código-fonte do cliente e gerar o executável "client".

4. Após a compilação bem-sucedida, você pode executar o cliente utilizando o comando:

   ```shell
   ./client
   ```

   Certifique-se de que o servidor esteja em execução antes de iniciar o cliente.

   Para encerrar a conexão, basta pressionar o botão 1 no teclado.

Lembre-se de adaptar as instruções conforme o software de virtualização e a plataforma específica que você está utilizando. Consulte a documentação relevante para obter mais informações sobre configuração e solução de problemas.

