using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace Morpion
{
    public partial class Fenetre2a : Form
    {
        Morpion fenetre2b; 
        Color couleur = Color.White; // Couleur qu'on a selectionné
        public Fenetre2a()
        {
            InitializeComponent();
        }

        // Fonction appelé lorsqu'on clicke sur "Demarrer"
        private void Click_Entrer(object sender, EventArgs e)
        {
            Parametres p = new Parametres();
            p.jeSuisServeur = checkBox_iAmHost.Checked;
            p.adrIP_serveur = textBox_adrIPserveur.Text;
     
            if (p.jeSuisServeur)
            {
                p.charServeur = Symbole_A.Text;
                p.colorServeur = couleur;
            }
            else
            {
                p.charClient = Symbole_A.Text;
                p.colorClient = couleur;
            }

            this.Hide(); // On cache la 1ere fenetre, pour pouvoir le réutiliser plus tard

            fenetre2b = new Morpion(p); // On lance la fenètre de jeu, avec les paramètres choisis
            fenetre2b.ShowDialog();

            this.Show();
        }
        
        
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            // Si on est serveur, on n'a pas à saisir l'adresse IP du client
            textBox_adrIPserveur.Enabled = !checkBox_iAmHost.Checked; 
        }
        private void Symbole_A_TextChanged(object sender, EventArgs e) {
            button_apparence.Text = Symbole_A.Text;
        }

        private void couleurJA_Click(object sender, EventArgs e)
        {
            if(colorDialog1.ShowDialog() != DialogResult.Cancel)
            {
                if(colorDialog1.Color == Color.Black )
                    MessageBox.Show("Vous ne pouvez pas choisir le noir.");
                else
                    button_apparence.BackColor = couleur = colorDialog1.Color;
            }
        }

        private void Fenetre2a_Load(object sender, EventArgs e) { }
        private void textBox1_TextChanged(object sender, EventArgs e) { }
        private void label2_Click(object sender, EventArgs e)
        { }
    }
}
