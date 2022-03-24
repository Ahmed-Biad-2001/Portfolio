using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Timers;

namespace Morpion
{
    public class ConnexionServeur : Connexion
    {
        private Jeu jeu = new Jeu();
        private TcpListener tcpListener;
        private Socket socket;
        private Timer timer_rejouer;

        public ConnexionServeur()
        {   
            // On écoute sur le port 4001
            tcpListener = new TcpListener(4001);
            tcpListener.Start();
            Console.WriteLine("Attente du client...");
            socket = tcpListener.AcceptSocket();


            if (!socket.Connected)
            {
                Console.WriteLine("La connexion a échouée!");
                connecte = false;
                return;
            }
            else
            {
                Console.WriteLine("Connexion étabie!");
                connecte = true;
                networkStream = new NetworkStream(socket);
                networkStream.ReadTimeout = 20; // Temps d'attente en ms pour recevoir une trame. Un temps court permettera de ne pas trop bloquer le programme

                // On initialise un timer de 3 secondes qui se déclenche lorsque la partie se termine, et qui réinitialise le jeu
                timer_rejouer = new System.Timers.Timer(3000);
                timer_rejouer.Elapsed += Rejouer;
                timer_rejouer.AutoReset = false;
            }
        }
        
        //
        // Transfère les informations au client
        
        public override void Update()
        {
            // Verifie si le client a envoye quelquechose
            LireTrame();

            // Transfert des informations au client

            string strGrille = "";
            for (int i = 0; i < 9; i++)
            {
                strGrille += (char)jeu.Grille[i];
            }

            string strTour = new string((char)jeu.Tour, 1);

            string strScoreS = jeu.scoreS.ToString();
            string strScoreC = jeu.scoreC.ToString();

            EcrireTrame(TrameType.EtatDuJeu, new string[] {strGrille, strTour, strScoreS, strScoreC} );

            // Mise à jour de l'interface
            jeu.Grille.CopyTo(etatFenetre.grille, 0);
            etatFenetre.tour = jeu.Tour;
            etatFenetre.scoreC = jeu.scoreC;
            etatFenetre.scoreS = jeu.scoreS;

        }

        private void Rejouer(object sender, EventArgs e)
        {
            jeu.InitialiserJeu();
        }

        // Si le serveur clicke sur une case, alors on appelle la fonction JouerCase avec comme argument le serveur
        public override void ClickCase(int c)
        {
            JouerCase(c, IdJoueur.Serveur);
        }

        private void JouerCase(int c, IdJoueur joueur)
        {
            // On vérifie si le joueur a bien son tour
            if (joueur == jeu.Tour)
            {   
                // On appelle la fonction JouerCase de la classe Jeu, et on voit le resultat
                int resultat = jeu.JouerCase(c);
                if (resultat == 2) // La partie est donc terminé
                {
                    timer_rejouer.Start(); // On recommence la partie dans 3 secondes
                }
                else if (resultat == 0) 
                {
                    Console.WriteLine("Erreur: La case jouee est invalide");
                }
            }
            else Console.WriteLine("Erreur: Le joueur n'a pas son tour");
        }

        protected override void TraiterTrame(Byte[] trame)
        {
            switch(trame[0])
            {
                case (byte)TrameType.CaseJouee: // Trame: "2 c"
                    int c = trame[2] -'0';
                    JouerCase(c, IdJoueur.Client); // Si le client clicke sur une case, alors on appelle la fonction JouerCase avec comme argument le client
                    break;
                case (byte)TrameType.Apparence:
                    TraiterTrameApparence(trame);
                    break;
            }

        }
        
    }
}


