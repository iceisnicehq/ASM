/*----------------------------------------------------------*/
/*                                                          */
/*   Turbo Vision TVPXENG source file                       */
/*                                                          */
/*----------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#define Uses_TApplication
#define Uses_TBackground
#define Uses_TButton
#define Uses_TKeys
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TScreen
#define Uses_TWindow
#define Uses_TWindowInit
#define Uses_TListBox
#define Uses_TMenu
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TRect
#define Uses_TScrollBar
#define Uses_TStaticText
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TStatusLine
#define Uses_TStringCollection
#define Uses_TCollection
#define Uses_TNSCollection
#define Uses_TInputLine
#define Uses_MsgBox
#include <tvision\tv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pxengine.h>

#define TABLE "TIS"

//
// Paradox Engine field names
//
char *fields[] =
{
  "TI Number",
  "Title",
};


//
// Paradox Engine field types
//
char *types[] =
{
    "N",
    "A95",
};

struct REC : public TObject
{
    RECORDNUMBER n;
    REC (RECORDNUMBER r) : n (r){}
};

//
// Paradox Engine error handling function.
//
void PXError (int x, char *file, long line)
{
    if (x != PXSUCCESS)
        {
         printf ("\nEngine Error # %d on line %ld in %s\n"
                 "Engine reports: %s\n", x, line, file, PXErrMsg (x));
         exit (-1);
        }
}

// Paradox Engine error handling macro.
#define pe(x) PXError (x, __FILE__, __LINE__)

// ----------------------------------------------------------------------------
//
//  global data
//
// ----------------------------------------------------------------------------

const cmAbout   = 100;  // User selected menu item 'About'
const cmList    = 101;  // User selected menu item 'List'

const cmMyNotification = 1001;
const cmMyNotification2 = 1002;
const cmMyValidation   = 1003;

#define cpColor1 "\x31\x1E\x6E\x1B\x0E\x6E"

// ----------------------------------------------------------------------------
//
//  class TApp
//   Application class.
//
// ----------------------------------------------------------------------------
class TApp : public TApplication {
    //  main application class

public:
    TApp();

    // virtual functions to be locally redefined
    static TMenuBar *initMenuBar( TRect r );
    void handleEvent( TEvent &event );

    // declare new functions
    void AboutDialog();
    void ListDialog();
};

// ----------------------------------------------------------------------------
//
//  Constructor TApp
//
// ----------------------------------------------------------------------------
TApp::TApp() : TProgInit( &TApplication::initStatusLine,
                    &TApp::initMenuBar, &TApplication::initDeskTop )
{
    // default status line and desk top
}

// ----------------------------------------------------------------------------
//
// Intialize menu bar
//
// ----------------------------------------------------------------------------
TMenuBar *TApp::initMenuBar( TRect r )
{
    r.b.y = r.a.y + 1;
    return( new TMenuBar( r,
        *new TSubMenu( "~ð~", 0, hcNoContext ) +
          *new TMenuItem( "~A~bout...", cmAbout, kbAltA, hcNoContext ) +
        *new TSubMenu( "~F~ile", 0, hcNoContext ) +
          *new TMenuItem( "~L~ist...", cmList, kbAltL, hcNoContext, 0 ) +
           newLine()+
          *new TMenuItem( "E~x~it", cmQuit, kbAltX, hcNoContext, "Alt-X")
        ) );
}

// ----------------------------------------------------------------------------
//
// TApplication event-handler
//
// ----------------------------------------------------------------------------
void TApp::handleEvent( TEvent &event )
{
    TApplication::handleEvent( event );
    if( event.what == evCommand )
    {
        switch( event.message.command )
        {
            case cmAbout:       // display the about box
                AboutDialog();
                clearEvent( event );
                break;
            case cmList:        // display our list box
                ListDialog();
                clearEvent( event );
                break;
        }
    }
}

//------------------------------------------------------------------------
//
// Create modal About dialog box
//
//------------------------------------------------------------------------
void TApp::AboutDialog()
{
    // the About box reminds the user what is being demonstrated
    TDialog *pd = new TDialog( TRect( 0, 0, 35, 13 ), "About" );
    if( pd )
    {
        pd->options |= ofCentered;
        pd->insert( new TStaticText( TRect( 1, 2, 34, 9 ),
                "\003Turbo Vision\n\003\n"
                "\003Paradox Engine Example\n\n"
                "\003Copyright (c) 1994\n\n"
                "\003Borland International" ) );
        pd->insert( new TButton( TRect( 11, 10, 23, 12 ), "~O~k",
                                cmOK, bfDefault ) );
        executeDialog( pd );
    }
}

class TRecCollection : public TCollection
{
  public:
   int def;
   FIELDHANDLE sort;
   TABLEHANDLE hand;

  TRecCollection (ushort a, ushort b);

  void writeItem( void *obj, opstream& os );

  void *readItem( ipstream& is );

};

// ----------------------------------------------------------------------------
//
// TRecCollection constructor.
//
// ----------------------------------------------------------------------------
TRecCollection::TRecCollection (ushort a, ushort b)
: TCollection (a,b), def (1), sort (1), hand (0)
{
}

// ----------------------------------------------------------------------------
//
// Stream support for write an item
//
// ----------------------------------------------------------------------------
void TRecCollection::writeItem( void *obj, opstream& os )
{
    os.writeString( (const char *)obj );
}

// ----------------------------------------------------------------------------
//
// Stream support for reading an item
//
// ----------------------------------------------------------------------------
void *TRecCollection::readItem( ipstream& is )
{
    return is.readString();
}


// ----------------------------------------------------------------------------
//
// Class TSpinnerWindow
//
// ----------------------------------------------------------------------------
class TSpinnerWindow : public TWindow
{
  public:

  int strptr;
  int times;

  TSpinnerWindow (char *s, int x = 1);

  virtual void draw(void);

  void Spin(void);
};

// ----------------------------------------------------------------------------
//
// TSpinnerWindow constructor.
//
// ----------------------------------------------------------------------------
TSpinnerWindow::TSpinnerWindow (char *s, int x)
: times (1), strptr (x), TWindow (TRect (1, 1, strlen(s)+12,5), s, 1),
 TWindowInit (&initFrame)
{
  flags  &= ~(wfMove | wfGrow | wfClose | wfZoom);
  options |= ofCentered;
}

// ----------------------------------------------------------------------------
//
// Draw spinner window
//
// ----------------------------------------------------------------------------
void TSpinnerWindow::draw(void)
{
  static char str[] = "|/-\\";
  TWindow::draw ();
  writeChar (10,1,str[strptr], 1, 1);
}

// ----------------------------------------------------------------------------
//
// Spin the character.
//
// ----------------------------------------------------------------------------
void TSpinnerWindow::Spin(void)
{
  static int count = 0;
  if (++count > times)
  {
    count = 0;
    strptr++;
    if (strptr > 3)
      strptr = 0;
    drawView ();
  }
}


// ----------------------------------------------------------------------------
//
// Class TTableDialog
//  Dialogbox displays a Paradox Table in a listbox control.
//
// ----------------------------------------------------------------------------
class TTableDialog : public TDialog
{
 public:
  TListBox *lb;

  TABLEHANDLE handle;
  RECORDHANDLE rec;

  TTableDialog (TRect &r, char *name);

  void ListInit (TListBox *p);

  ~TTableDialog (void);

  virtual void handleEvent(TEvent &e);
};

// ----------------------------------------------------------------------------
//
// Constructor
//
// ----------------------------------------------------------------------------
TTableDialog::TTableDialog (TRect &r, char *name)
: TDialog (r, name), TWindowInit( initFrame )
{
 lb = NULL;

#ifdef __DPMI16__
 pe (PXWinInit ("USER",PXSINGLECLIENT));
#else
 pe (PXInit());
#endif

 pe ((PXTblOpen (TABLE, &handle, 0,0)));
 PXRecBufOpen (handle, &rec);
}

// ----------------------------------------------------------------------------
//
// TTableDialog destructor.
//
// ----------------------------------------------------------------------------
TTableDialog::~TTableDialog (void)
{
 pe (PXRecBufClose (rec));
 pe (PXTblClose (handle));
 pe (PXExit ());
}

// ----------------------------------------------------------------------------
//
// Initalize the listbox control with Paradox Engine table data.
//
// ----------------------------------------------------------------------------
void TTableDialog::ListInit (TListBox *p)
{
 RECORDNUMBER num = 0;
 lb = p;

 pe (PXRecFirst (handle));
 ((TRecCollection *)lb->list())->hand = handle;
 ((TRecCollection *)lb->list())->sort = 1;
 TSpinnerWindow *spin = new TSpinnerWindow ("Loading data...", 10);
 TProgram::deskTop->insert (spin);
 do
 {
  pe (PXRecNum (handle, &num));
  lb->list()->insert(new REC(num));
  lb->setRange(lb->range+1);
  spin->Spin ();
 }
 while (!PXRecNext (handle));
  destroy (spin);
}


// ----------------------------------------------------------------------------
//
// TTableDialog handle event
//
// ----------------------------------------------------------------------------
void TTableDialog::handleEvent (TEvent &e)
{
  RECORDNUMBER num;
  static char title[96];
  static char temp[256];
  long ti_num;

  if (e.what == evBroadcast)
    if (e.message.command == cmMyNotification)
    {
      if (lb->range > 0)
      {
        num = ((REC*)(lb->list()->at(lb->focused)))->n;
        if (PXRecGoto (handle, num) == PXSUCCESS)
        {
          pe (PXRecGet (handle, rec));
          pe (PXGetAlpha (rec, 2, 95, title));
          pe (PXGetLong (rec, 1, &ti_num));
          sprintf (temp, "%ld -- %s", ti_num, title);
        }
        else
          strcpy (temp, "<Error finding record>");

        messageBox (mfInformation, "item:%d\n%s", lb->focused, temp);
        redraw();
      }
    }

  TDialog::handleEvent (e);
}



// ----------------------------------------------------------------------------
//
// Class TTableListBox
//  Class displays a listbox of a Paradox Engine database
//
// ----------------------------------------------------------------------------
class TTableListBox : public TListBox
{
 public:
  TTableListBox (TRect &r, ushort n, TScrollBar *s);

  virtual void getText (char *dest, short item, short maxLen);

  virtual void handleEvent (TEvent &e);

};

// ----------------------------------------------------------------------------
//
// TTableListBox constructor.
//
// ----------------------------------------------------------------------------
TTableListBox::TTableListBox (TRect &r, ushort n, TScrollBar *s)
: TListBox (r,n,s)
{
}

// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------
void TTableListBox::getText (char *dest, short item, short maxLen)
{
  RECORDNUMBER num;
  static char title[96];
  static char temp[256];
  long ti_num;
  TTableDialog *parent;
  REC *record;

  parent = (TTableDialog *)owner;
  TRecCollection *trCol;

  trCol = (TRecCollection *)list();
  record = (REC *)trCol->at(item);
  num = record->n;

  if (PXRecGoto (parent->handle, num) == PXSUCCESS)
  {
    pe (PXRecGet (parent->handle, parent->rec));
    pe (PXGetAlpha (parent->rec, 2, 95, title));
    pe (PXGetLong (parent->rec, 1, &ti_num));
    sprintf (temp, "%ld - %s", ti_num, title);
    if (maxLen < 80)
      temp[maxLen-1] = 0;
    strcpy (dest, temp);
  }
  else
    *dest = EOS;
}

// ----------------------------------------------------------------------------
//
// Listbox handle event function.
//
// ----------------------------------------------------------------------------
void TTableListBox::handleEvent (TEvent &e)
{
      if( e.what == evKeyDown )
      {
        if (e.keyDown.keyCode == kbEnter)
          message (owner, evBroadcast, cmMyNotification, this);
      }
      else
      if (e.what == evMouseDown)
      {
        if (e.mouse.eventFlags == meDoubleClick)
          message (owner, evBroadcast, cmMyNotification, this);
      }

      TListBox::handleEvent (e);
}



//------------------------------------------------------------------------
//
//  Display a dialog box containing a listbox
//
//------------------------------------------------------------------------
void TApp::ListDialog()
{
    TRecCollection *theCollection = new TRecCollection( 500, 10 );

    TDialog *pd = new TTableDialog( TRect( 0, 0, 76, 20 ), "Table View" );
    pd->options |= ofCentered;

    // Save the table handle for future use
    theCollection->hand = ((TTableDialog *)pd)->handle;

    // create a vertical scroll bar for the list box
    TScrollBar *listScroller = new TScrollBar( TRect( 74, 1, 75, 19 ) );

    // create a list box with 2 columns and associated scroller
    TTableListBox *listBox = new TTableListBox( TRect( 1, 1, 74, 19 ),
                                        1, listScroller );

    // associate the collection with the list box
    listBox->newList( (TCollection *) theCollection );

    // insert the list box with its scroller into the dialog box

    pd->insert( listBox );
    pd->insert( listScroller );


    ((TTableDialog *)pd)->ListInit(listBox);  // Load up the list box

    // if the object was safely built, exec it as modal
    if( validView( pd ) )
        deskTop->execView( pd );

    // and a little clean up
    destroy( pd );
}


//
// main
//
int main(void)
{
    // now for the tought stuff
    TApp application;
    application.run();

    return 0;
}
