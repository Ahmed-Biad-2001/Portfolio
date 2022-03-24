using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;

namespace Morpion
{
    public class ConnexionClient : Connexion
    {
        TcpClient client = null;
        public ConnexionClient(string adrIP)
        {   
            // On se connecte à l'adresse IP du serveur spécifiée au port 4001
            client = new TcpClient(adrIP, 4001);

            if (client.Connected)
            {
                Console.WriteLine("Connexion étabie!");
                networkStream = client.GetStream();
                networkStream.ReadTimeout = 20; // Temps d'attente en ms pour recevoir une trame. Un temps court permettera de ne pas trop bloquer le programme
                connecte = true;
            }
            else
            {
                Console.WriteLine("La connexion a échouée!");
                connecte = false;
            }
        }

        // On fait les actions nécessaires selon la trame récupérée
        protected override void TraiterTrame(Byte[] trame) {
            switch (trame[0]) // On vérifie c'est quel type de trame
            {
                case (byte)TrameType.EtatDuJeu: 
                    // On copie les informations du jeu envoyés par le serveur
                    for (int i=2; i<11; i++)
                    {
                        etatFenetre.grille[i-2] = (IdJoueur)trame[i];
                        
                    }
                    etatFenetre.tour = (IdJoueur)trame[12]; 

                    etatFenetre.scoreS = trame[14] - '0';
                    etatFenetre.scoreC = trame[16] - '0';
                    break;
                case (byte)TrameType.Apparence:
                    TraiterTrameApparence(trame);
                    break;
            }
        }

        public override void ClickCase(int c)
        {
            // On envoie au serveur le fait qu'on a clické sur une case
            EcrireTrame(TrameType.CaseJouee, new string[] { c.ToString() } );
        }

        public override void Update() {
            // On vérifie si on a reçu une trame périodiquement
            LireTrame();
        }

    }
}
