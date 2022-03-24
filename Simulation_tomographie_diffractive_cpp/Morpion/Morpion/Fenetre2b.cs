using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Timers;
using System.Threading;

namespace Morpion
{
    public partial class Morpion : Form
    {
        // Objet d'une classe, qui va permettre de communiquer avec l'autre joueur, et donner la fenetre les informations à afficher
        private Connexion connexion = null;
        Parametres parametres;
        
        // Timer qui permet de mettre à jour le jeu péridiquement
        private static System.Timers.Timer timer_miseajour;

        EventArgs e = null;

        public Morpion(Parametres p)
        {
            InitializeComponent();
            parametres = p;

            // On execute le constructeur de la classe fille correspondate
            if (p.jeSuisServeur == true)
            {
                this.Text = "Morpion (Serveur)";
                connexion = new ConnexionServeur();
            }
            else
            {
                this.Text = "Morpion (Client)";
                connexion = new ConnexionClient(parametres.adrIP_serveur);
            }

            if(connexion.connecte == false)
            {
                Console.WriteLine("La connexion a échoué, on ferme la fenêtre");
                this.Close();
            }
            
            timer_miseajour = new System.Timers.Timer(200); // On rafraichit tous les 200 ms
            timer_miseajour.Elapsed += this.MiseAJour; // La fonction MiseAJour() est appelée à l'écoulement du timer
            timer_miseajour.SynchronizingObject = this; // Le timer fait appel à MiseAJour dans un nouveau thread, or celui-ci accède à une zone mémoire partagée.
                                                        // Du coup, il faut rajouter cette ligne, pour que il y accède au bon moment.
            timer_miseajour.AutoReset = true;
            timer_miseajour.Enabled = true;


            // On commence par envoyer à l'autre machine une trame, pour lui passer notre couleur et pseudo.
            if (parametres.jeSuisServeur)
                connexion.EcrireTrameApparence(p.charServeur, p.colorServeur);
            else
                connexion.EcrireTrameApparence(p.charClient, p.colorClient);

            // On attend de recevoir la couleur et le pseudo de l'adversaire
            while (connexion.pseudoInitialise == false)
            {
                Thread.Sleep(200);
            }
            
            // On peut maintenant récuperer sa couleur et son pseudo
            if(parametres.jeSuisServeur)
            {
                parametres.charClient = connexion.pseudoAdverse;
                parametres.colorClient = connexion.couleurAdverse;
            }
            else
            {
                parametres.charServeur = connexion.pseudoAdverse;
                parametres.colorServeur = connexion.couleurAdverse;
            }

            JoueurC.Text = parametres.charClient;
            JoueurS.Text = parametres.charServeur;
        }

        // Cette fonction est appelée periodiquement, et permet de rafraichir l'interface
        private void MiseAJour(object sender, EventArgs e)
        {
            // On fait d'abord les échanges réseau nécessaire
            connexion.Update();
            
            for (int i = 0; i < 9; i++)
            {
                string strC = "c" + i.ToString();  // strC = "c0", "c1", "c2", ...
                                                   // On peut accéder aux boutons de la grille on se servant du tableau Controls,
                                                   //   qui est un tableau regroupant tous les élements de la fenêtre
                                                   
                // On change l'apparence de la case selon le joueur qui y a joué
                if (connexion.etatFenetre.grille[i] == IdJoueur.Client)
                {
                    Controls[strC].Text = parametres.charClient;
                    Controls[strC].BackColor = parametres.colorClient;
                    Controls[strC].Enabled = false;
                }
                else if (connexion.etatFenetre.grille[i] == IdJoueur.Serveur)
                {
                    Controls[strC].Text = parametres.charServeur;
                    Controls[strC].BackColor = parametres.colorServeur;
                    Controls[strC].Enabled = false;
                }
                else
                {
                    Controls[strC].Text = "";
                    Controls[strC].BackColor = Color.White;
                    Controls[strC].Enabled = true;
                }
                
            // On active/désactive les boutons selon si on a le tour ou pas
                if (connexion.etatFenetre.tour == IdJoueur.Client && parametres.jeSuisServeur
                || connexion.etatFenetre.tour == IdJoueur.Serveur && !parametres.jeSuisServeur)
                    Controls[strC].Enabled = false;
            }
            
            // On dit à qui c'est le tour
            if (connexion.etatFenetre.tour == IdJoueur.Serveur) 
                label_textTour.Text = "C'est le tour de " + parametres.charServeur;
            else label_textTour.Text = "C'est le tour de " + parametres.charClient;

            // On affiche le score
            ScoreC.Text = connexion.etatFenetre.scoreC.ToString();
            ScoreS.Text = connexion.etatFenetre.scoreS.ToString();
        }

        // Lorsqu'on appuie sur une case de la grille: c'est l'objet Connexion qui va effectuer tous les operations nécessaires
        private void bouton_click(object sender, EventArgs e )
        {
            Button mon_bouton = (Button)sender; // Désigne un des 9 boutons
            int c=-1;
            c = (int)mon_bouton.Name[1] - '0'; // Conversion de char en int

            if (c >= 0 && c <= 8) connexion.ClickCase(c);
        }



        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Morpion_Load(object sender, EventArgs e)
        { }
        
        private void Gagnant()
        {
            //MessageBox.Show(message + " à gagné !!!");
        }

        
    }
}
