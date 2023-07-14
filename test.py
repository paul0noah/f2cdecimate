import igl
v, _, _, f, _, _ = igl.read_obj("tr_reg_080.obj")

import fine2coarsedec as f2cd
vred, fred, vf2c, ff2c = f2cd.decimate(v, f, 100, False)

print(vred)
import polyscope as ps

ps.init()
ps.register_surface_mesh('full', v, f)
ps.register_surface_mesh('red', vred, fred)
ps.show()
