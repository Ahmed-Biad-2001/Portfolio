
namespace Morpion
{
    partial class Fenetre2a
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Symbole_A = new System.Windows.Forms.TextBox();
            this.Message_A = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.couleurJA = new System.Windows.Forms.Button();
            this.checkBox_iAmHost = new System.Windows.Forms.CheckBox();
            this.textBox_adrIPserveur = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.button_apparence = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // Symbole_A
            // 
            this.Symbole_A.Location = new System.Drawing.Point(145, 16);
            this.Symbole_A.MaxLength = 4;
            this.Symbole_A.Name = "Symbole_A";
            this.Symbole_A.Size = new System.Drawing.Size(104, 20);
            this.Symbole_A.TabIndex = 0;
            this.Symbole_A.TextChanged += new System.EventHandler(this.Symbole_A_TextChanged);
            // 
            // Message_A
            // 
            this.Message_A.AutoSize = true;
            this.Message_A.Location = new System.Drawing.Point(32, 19);
            this.Message_A.Name = "Message_A";
            this.Message_A.Size = new System.Drawing.Size(107, 13);
            this.Message_A.TabIndex = 2;
            this.Message_A.Text = "Saisir votre caractère";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(133, 240);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(125, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "Démarrer";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.Click_Entrer);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(32, 55);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(151, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Quelle couleur préférez-vous ?";
            // 
            // couleurJA
            // 
            this.couleurJA.Location = new System.Drawing.Point(201, 50);
            this.couleurJA.Name = "couleurJA";
            this.couleurJA.Size = new System.Drawing.Size(120, 23);
            this.couleurJA.TabIndex = 7;
            this.couleurJA.Text = "Choix de la couleur Joueur A";
            this.couleurJA.UseVisualStyleBackColor = true;
            this.couleurJA.Click += new System.EventHandler(this.couleurJA_Click);
            // 
            // checkBox_iAmHost
            // 
            this.checkBox_iAmHost.AutoSize = true;
            this.checkBox_iAmHost.Location = new System.Drawing.Point(35, 178);
            this.checkBox_iAmHost.Name = "checkBox_iAmHost";
            this.checkBox_iAmHost.Size = new System.Drawing.Size(107, 17);
            this.checkBox_iAmHost.TabIndex = 8;
            this.checkBox_iAmHost.Text = "Je suis le serveur";
            this.checkBox_iAmHost.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            this.checkBox_iAmHost.UseVisualStyleBackColor = true;
            this.checkBox_iAmHost.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // textBox_adrIPserveur
            // 
            this.textBox_adrIPserveur.Location = new System.Drawing.Point(181, 208);
            this.textBox_adrIPserveur.Name = "textBox_adrIPserveur";
            this.textBox_adrIPserveur.Size = new System.Drawing.Size(104, 20);
            this.textBox_adrIPserveur.TabIndex = 0;
            this.textBox_adrIPserveur.Text = "localhost";
            this.textBox_adrIPserveur.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(64, 211);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(111, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Adresse IP du serveur";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // button_apparence
            // 
            this.button_apparence.Enabled = false;
            this.button_apparence.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F);
            this.button_apparence.Location = new System.Drawing.Point(156, 79);
            this.button_apparence.Name = "button_apparence";
            this.button_apparence.Size = new System.Drawing.Size(75, 75);
            this.button_apparence.TabIndex = 9;
            this.button_apparence.UseVisualStyleBackColor = true;
            // 
            // Fenetre2a
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(387, 284);
            this.Controls.Add(this.button_apparence);
            this.Controls.Add(this.checkBox_iAmHost);
            this.Controls.Add(this.couleurJA);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBox_adrIPserveur);
            this.Controls.Add(this.Message_A);
            this.Controls.Add(this.Symbole_A);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Fenetre2a";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Morpion en ligne";
            this.Load += new System.EventHandler(this.Fenetre2a_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox Symbole_A;
        private System.Windows.Forms.Label Message_A;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.Button couleurJA;
        private System.Windows.Forms.CheckBox checkBox_iAmHost;
        private System.Windows.Forms.TextBox textBox_adrIPserveur;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button_apparence;
    }
}