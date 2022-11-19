                                                                                                                 
                            ███╗                                                                                 
                          ███╔═╝                                                                                 
                          ╚══╝                                                                                   
██████╗  ██████╗ ██╗  ██╗███████╗    ██╗   ██╗███╗   ██╗██╗██╗   ██╗███████╗██████╗ ███████╗██╗████████╗██╗   ██╗
██╔══██╗██╔═══██╗██║ ██╔╝██╔════╝    ██║   ██║████╗  ██║██║██║   ██║██╔════╝██╔══██╗██╔════╝██║╚══██╔══╝╚██╗ ██╔╝
██████╔╝██║   ██║█████╔╝ █████╗█████╗██║   ██║██╔██╗ ██║██║██║   ██║█████╗  ██████╔╝███████╗██║   ██║    ╚████╔╝ 
██╔═══╝ ██║   ██║██╔═██╗ ██╔══╝╚════╝██║   ██║██║╚██╗██║██║╚██╗ ██╔╝██╔══╝  ██╔══██╗╚════██║██║   ██║     ╚██╔╝  
██║     ╚██████╔╝██║  ██╗███████╗    ╚██████╔╝██║ ╚████║██║ ╚████╔╝ ███████╗██║  ██║███████║██║   ██║      ██║   
╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝     ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝╚══════╝╚═╝   ╚═╝      ╚═╝   
                                                                                                                 

Afin de puvoir éxécuter le client il faut d'abord le compiler pour cela un makefile est à votre disposition.
Il suffit d'utiliser la commande make et encuite dans le fichier build qui sera créer se trouvera l'executable "client".
Deux autres éxécutables s'y trouvent aussi: "team" et "tchat" ses éxécutable sont utiliser par le client et ne peuvent être utilisé que par lui.
L'executable ne fonctionne aussi que sous linux à cause de l'utilisation de fifo.