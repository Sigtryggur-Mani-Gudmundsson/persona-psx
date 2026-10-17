#include <persona/casino/casino.h>

extern void CasinoTexObj(CasinoModel *);
extern void CasinoSetXform(CasinoXform *);
extern void CasinoDrawObj(CasinoObj *);

void func_80092DEC(CasinoModel *model, CasinoXform xform)
{
    model->obj->xform->rot.vx = xform.rot.vx;
    model->obj->xform->rot.vy = xform.rot.vy;
    model->obj->xform->rot.vz = xform.rot.vz;
    model->obj->xform->trans.vx = xform.trans.vx;
    model->obj->xform->trans.vy = xform.trans.vy;
    model->obj->xform->trans.vz = xform.trans.vz;
    model->obj->xform->scale.vx = xform.scale.vx;
    model->obj->xform->scale.vy = xform.scale.vy;
    model->obj->xform->scale.vz = xform.scale.vz;
    CasinoTexObj(model);
    CasinoSetXform(model->obj->xform);
    CasinoDrawObj(model->obj);
}
