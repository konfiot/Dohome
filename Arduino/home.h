#ifndef HOME_H
#define HOME_H

#include "vector.h"
#include "piece.h"

class Home
{
public:
                      Home            (const char     *name    );
    bool              init            (                        );
    bool              addPiece        (Piece          &piece   );
    bool              fillJSONData    (BufferFiller   &buf     );
    bool              refresh         (                        );
    virtual bool      prepare         (const char     *arg     , byte id_p, byte id_a);
    virtual bool      exec            (                        );
                      ~Home           (                        );
                      
private:
    const char*       _name;
    byte              _prepared_id;
    Vector<Piece*>    _pieces;
};

#endif // HOME_H
