VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox lstSections 
      Height          =   1815
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   2055
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Declare Function shlib_CreateHero Lib "shlib.dll" () As Long
Private Declare Sub shlib_DestroyHero Lib "shlib.dll" (ByVal hHero As Long)
Private Declare Function shlib_CompileHero Lib "shlib.dll" (ByVal hHero As Long, ByVal szToPath As String) As Boolean
Private Declare Function shlib_DecompileHero Lib "shlib.dll" (ByVal hHero As Long, ByVal szHeroPath As String) As Boolean
Private Declare Function shlib_GetHeroSection Lib "shlib.dll" (ByVal hHero As Long, ByVal lSectionType As Long, ByRef lpBuffer As String) As Long
Private Declare Function shlib_SetHeroSection Lib "shlib.dll" (ByVal hHero As Long, ByVal lSectionType As Long, ByVal lpBuffer As String, ByVal nBuffSize As Long) As Boolean
Private Declare Function shlib_Compress Lib "shlib.dll" (ByVal dest As String, ByRef destLen As Long, ByVal source As String, ByVal sourceLen As Long, ByVal level As Integer) As Long
Private Declare Function shlib_Uncompress Lib "shlib.dll" (ByVal dest As String, ByRef destLen As Long, ByVal source As String, ByVal sourceLen As Long) As Long
Private Declare Function shlib_GetSectionsList Lib "shlib.dll" (ByVal hHero As Long, ByVal lpulArray As Long) As Integer

Private Sub Form_Load()
    Dim hHero As Long
    
    ' test CreateHero... creates a HHERO instance... this allows you to open multiple heros at once
    hHero = shlib_CreateHero()
    
    ' test DecompileHero... decomiles the PAX file
    Dim rc As Integer: rc = shlib_DecompileHero(hHero, "c:\Hero04.pax")
    
    ' test GetSectionsList... gets a list of the sections of this hero
    ' if the second param is null, then it just returns how many sections there are
    Dim nTotalSections As Integer
    nTotalSections = shlib_GetSectionsList(hHero, 0)
    
    ' now we pass in an array of long integers to get the section names
    Dim anarray() As Long
    ReDim anarray(nTotalSections - 1) As Long
    shlib_GetSectionsList hHero, VarPtr(anarray(0))
    
    ' display the section names in the list box
    Dim q As Integer
    For q = 0 To nTotalSections - 1
        lstSections.AddItem anarray(q) & " [Hex: " & Hex(anarray(q)) & "]"
    Next q
    
    ' test GetHeroSection (on section 0xC8 [200]).. passing NULL in gets the size of the section
    Dim i As Long: i = shlib_GetHeroSection(hHero, &HC8, vbNullString)
    ' if shlib_GetHeroSection() fails, i will equal -1, otherwise its the size of the section
    
    ' allocate memory
    Dim str As String: str = String(i, Chr(0))
    ' get the actual contents of the section
    i = shlib_GetHeroSection(hHero, &HC8, str)
    
    '
    ' this is when you edit the section how you want....
    '
    
    ' test SetHeroSection... after you edit the section, this will replace it
    rc = shlib_SetHeroSection(hHero, &HC8, str, Len(str))
    
    ' test CompileHero... after you finish editing it, this will recompile the hero file
    rc = shlib_CompileHero(hHero, "C:\Hero04_backup.pax")
    
    ' test DestryHero... we are done with hHero, so get rid of it's instance
    shlib_DestroyHero hHero
    
    '
    ' the following functions test the DLLs gzip capabilities
    '
    
    ' test Compress
    Dim strr As String, comp As Long
    strr = String(1024, Chr(0))
    comp = 1024
    Dim abc As Long: abc = shlib_Compress(strr, comp, "hellooooooooooooooooooooo thereeeeeeeeeeeeee!", Len("hellooooooooooooooooooooo thereeeeeeeeeeeeee!"), 9)
    
    ' test Uncompress
    Dim destt As String
    destt = String(1024, Chr(0))
    Dim desttlen As Long
    desttlen = 1024
    abc = shlib_Uncompress(destt, desttlen, strr, comp)
    
End Sub
