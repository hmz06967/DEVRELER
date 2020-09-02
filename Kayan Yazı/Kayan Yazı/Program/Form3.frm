VERSION 5.00
Begin VB.Form Form3 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Zaman Ayarla"
   ClientHeight    =   1545
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   3450
   Icon            =   "Form3.frx":0000
   LinkTopic       =   "Form3"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1545
   ScaleWidth      =   3450
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame Frame1 
      Caption         =   "Saat-Tarih Ayarla"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   162
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   1335
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   3255
      Begin VB.Timer Timer1 
         Interval        =   500
         Left            =   120
         Top             =   840
      End
      Begin VB.CommandButton Command1 
         Caption         =   "&AYARLA"
         Height          =   255
         Left            =   480
         TabIndex        =   3
         Top             =   840
         Width           =   1935
      End
      Begin VB.TextBox Text1 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   162
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   120
         Locked          =   -1  'True
         TabIndex        =   2
         Top             =   360
         Width           =   1455
      End
      Begin VB.TextBox Text2 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   162
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   1680
         Locked          =   -1  'True
         TabIndex        =   1
         Top             =   360
         Width           =   1455
      End
   End
End
Attribute VB_Name = "Form3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim Gun As Integer
Dim Ay As Integer
Dim Yil As Integer
Dim Saat As Integer
Dim Dakika As Integer
Dim Saniye As Integer
Dim i As Integer
Dim S_Say As Integer

Public Function Cevir(Sayi)
    Select Case Sayi
        Case 10
            Cevir = 16
        Case 11
            Cevir = 17
        Case 12
            Cevir = 18
        Case 13
            Cevir = 19
        Case 14
            Cevir = 20
        Case 15
            Cevir = 21
        Case 16
            Cevir = 22
        Case 17
            Cevir = 23
        Case 18
            Cevir = 24
        Case 19
            Cevir = 25
        Case 20
            Cevir = 26
        Case 21
            Cevir = 27
        Case 22
            Cevir = 28
        Case 23
            Cevir = 29
        Case 24
            Cevir = 30
        Case 25
            Cevir = 31
        Case 26
            Cevir = 38
        Case 27
            Cevir = 39
        Case 28
            Cevir = 40
        Case 29
            Cevir = 41
        Case 30
            Cevir = 42
        Case 31
            Cevir = 43
        Case 32
            Cevir = 44
        Case 33
            Cevir = 45
        Case 34
            Cevir = 46
        Case 35
            Cevir = 47
        Case 36
            Cevir = 54
        Case 37
            Cevir = 55
        Case 38
            Cevir = 56
        Case 39
            Cevir = 57
        Case 40
            Cevir = 58
        Case 41
            Cevir = 59
        Case 42
            Cevir = 60
        Case 43
            Cevir = 61
        Case 44
            Cevir = 62
        Case 45
            Cevir = 63
        Case 46
            Cevir = 70
        Case 47
            Cevir = 71
        Case 48
            Cevir = 72
        Case 49
            Cevir = 73
        Case 50
            Cevir = 74
        Case 51
            Cevir = 75
        Case 52
            Cevir = 76
        Case 53
            Cevir = 77
        Case 54
            Cevir = 78
        Case 55
            Cevir = 79
        Case 56
            Cevir = 86
        Case 57
            Cevir = 87
        Case 58
            Cevir = 88
        Case 59
            Cevir = 89
        
        Case Else
            Cevir = Sayi
    End Select
End Function

Private Sub Command1_Click()
    On Error GoTo Hata:
    
    Gun = Mid(Now, 1, 2)
    Ay = Mid(Now, 4, 2)
    Yil = Mid(Now, 9, 2)
    
    Saat = Mid(Now, 12, 2)
    Dakika = Mid(Now, 15, 2)
    Saniye = Mid(Now, 18, 2)
    
    
    
    Form1.MSComm1.PortOpen = True
        Form1.MSComm1.Output = Chr(250)
        Form1.MSComm1.Output = Chr(Cevir(Saniye))
        Form1.MSComm1.Output = Chr(Cevir(Dakika))
        Form1.MSComm1.Output = Chr(Cevir(Saat))
        Form1.MSComm1.Output = Chr(Cevir(Gun))
        Form1.MSComm1.Output = Chr(Cevir(Ay))
        Form1.MSComm1.Output = Chr(1)
        Form1.MSComm1.Output = Chr(Cevir(Yil))
    Form1.MSComm1.PortOpen = False
    
    
    
    MsgBox "Panonun Saati Ve Tarihi Bilgisayarýnki Ýle Ayný Yapýldý", 64, "Zaman Ayarlandý"
    Exit Sub
Hata:
    MsgBox "Ayarlama Sýrasýnda Bir Hata Oluþtu Port BÝlgilerini Kontrol Ediniz", 64, "Ayarlama Yapýlamadý."
    Exit Sub
    
End Sub

Private Sub Form_Load()
    Text1.Text = Mid(Now, 1, 10) 'Tarihi Yaz
    Text2.Text = Mid(Now, 11, 9) 'Saati Yaz
End Sub

Private Sub Timer1_Timer()
    Text1.Text = Mid(Now, 1, 10) 'Tarihi Yaz
    Text2.Text = Mid(Now, 11, 9) 'Saati Yaz
End Sub

