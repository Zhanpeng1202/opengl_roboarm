#version 120

varying vec4 color;

void main()
{
	gl_FragColor = color;
}



// #version 120


// varying vec2 texCoord;
// varying vec4 N, V, L;

// uniform sampler2D texture;

// vec4 ambient, diffuse, specular;

// varying float distance;

// uniform float shininess;
// uniform float attenuation_constant, attenuation_linear, attenuation_quadratic;

// uniform int ambiant_on, diffuse_on, specular_on;


// vec4 ambiant_color;

// void main()
// {
//     vec4 the_color = texture2D(texture, texCoord);
//     vec4 NN = normalize(N);
//     vec4 LL = normalize(L);
//     vec4 VV = normalize(V);
//     vec4 H = normalize(LL + VV);
//     float attenuation = 1;

//     ambient = the_color * 0.8;
//     diffuse = max(dot(LL, NN), 0.0) * 1.4 * the_color;
//     specular = pow(max(dot(NN, H), 0.0), shininess) * vec4(1.0, 1.0, 1.0, 1.0);
//     attenuation = 1/(attenuation_constant + (attenuation_linear * distance) + (attenuation_quadratic * distance * distance));
//     gl_FragColor =  (ambiant_on * ambient + attenuation * (diffuse_on * diffuse + specular_on * ambiant_color * specular));
    
// }