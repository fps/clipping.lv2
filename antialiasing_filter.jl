import Pkg
Pkg.activate(".")
import DSP
import Printf

f = DSP.Filters.digitalfilter(DSP.Filters.Lowpass(0.2), DSP.Filters.Elliptic(6, 1, 72))

g = convert(DSP.Filters.SecondOrderSections, f)

coeffs = hcat(map(x -> [x.b0, x.b1, x.b2, x.a1, x.a2], g.biquads)...)'

Printf.@printf("const float coefficients[] = {\n")
for row in 1:size(coeffs,1)
    Printf.@printf("  ")
    for col in 1:size(coeffs,2)
        Printf.@printf("%.32f, ", coeffs[row, col])
    end
    Printf.@printf("\n")
end
Printf.@printf("};\n")

Printf.@printf("const float gain = %.32f;\n", g.g)
