import numpy as np
import matplotlib.pyplot as plt
a = np.array([(0, 0, -0.0184303),
(-3.76784e-05, 0.000107843, -0.0294521),
(0.0145755, -0.00312947, -0.0557008),
(-0.00317608, -0.00962795, -0.101487),
(-0.00317374, -0.00987305, -0.028358),
(-0.00155138, -0.00654347, -0.118145),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, -0.0285625),
(-0.00597788, -0.0108684, 0.00181717),
(-0.0041942, -0.0109741, 0.00181717),
(0.00289677, -0.0109652, 0.00181717),
(0.0201376, -0.0109436, 0.00181717),
(0.0482155, -0.0109084, 0.00181717),
(0.09429, -0.0108506, 0.00181717),
(0.14573, -0.010786, 0.00181717),
(0.193955, -0.0107256, 0.00181717),
(0.251502, -0.0106534, 0.00181717),
(0.317238, -0.0105709, 0.00181717),
(0.393057, -0.0104758, 0.00181717),
(0.459957, -0.0103919, 0.00181717),
(0.526857, -0.010308, 0.00181717),
(0.598217, -0.0102185, 0.00181717),
(0.665116, -0.0101346, 0.00181717),
(0.727556, -0.0100563, 0.00181717),
(0.800396, 0, 3.8337e-12),
(0.867296, 0, 3.8337e-12),
(0.938656, 0, 3.8337e-12),
(1.0011, 0, 3.8337e-12),
(1.068, 0, 3.8337e-12),
(1.13489, 0, 3.8337e-12),
(1.20179, 0, 3.8337e-12),
(1.26869, 0, 3.8337e-12),
(1.33559, 0, 3.8337e-12),
(1.40249, 0, 3.8337e-12),
(1.47385, 0, 3.8337e-12),
(1.54075, 0, 3.8337e-12),
(1.60765, -1.73857e-11, -1.23812e-09),
(1.67739, -0.0064924, -0.0525061),
(1.67489, -0.00607415, -0.0816502),
(1.78522, -0.0158196, -0.0468101),
(1.87, -0.0210116, -0.0468388),
(1.94143, -0.025134, -0.0468101),
(2.02619, -0.0304003, -0.0467643),
(2.12873, -0.0372117, -0.0467883),
(2.22241, -0.0430386, -0.0467995),
(2.31165, -0.0484641, -0.0467765),
(2.40976, -0.0548136, -0.0467883),
(2.50344, -0.0606405, -0.0467883),
(2.59715, -0.0663464, -0.0467514),
(2.70536, -0.0626468, -0.0424193),
(2.80798, -0.0681416, -0.0425806),
(2.90617, -0.0732426, -0.0422865),
(3.01323, -0.0790796, -0.042188),
(3.12032, -0.0848133, -0.0421093),
(3.2274, -0.090547, -0.0933128),
(3.33437, -0.0980153, -0.0417766),
(3.44137, -0.105381, -0.0422296),
(3.56113, -0.0687531, -0.0427297),
(3.55104, -0.0086881, -0.01823),
(3.73113, -0.0371569, -0.00812548),
(5.06923, -0.968365, 0.263974),
(4.85997, -0.972986, 0.100575),
(4.37083, -0.0234159, 0.00627343),
(4.6469, 0.00325315, 0.0261556),
(4.83161, -0.00207083, -2.4222e-05),
(5.09338, -0.0399431, 0.00420864),
(5.27256, -0.0014833, 0.000979994),
(5.48469, -0.000298677, 0.0184374),
(5.67694, -0.000414313, 0.000451931),
(5.91647, -0.000315117, 0.0184026),
(6.1387, -0.000250919, 0.00013639),
(6.41898, -0.0018815, 0.0150186),
(6.67376, -0.00596157, 0.0271862),
(6.93595, -0.00715327, -0.00470474),
(7.18393, 0.0031575, 0.0244954),
(7.40959, 0.00481545, 0.00427066),
(7.65189, 0.0104226, 0.00664405),
(7.89422, 0.0148948, 0.00651847),
(8.11425, 0.0134375, -0.0204062),
(8.34839, 0.0285002, -0.054715),
(8.56836, 0.0518998, -0.116429),
(8.80714, 0.0646324, 0.0175653),
(9.17405, 0.0821125, 0.00885441),
(9.50894, 0.054869, 0.00442735),
(9.69846, -0.802887, 0.661609),
(10.3151, 0.017791, 0.0962482),
(10.7513, 0.208626, 0.104555),
(11.1432, 0.213374, 0.0978248),
(11.6382, 0.315918, 0.155328),
(12.0687, 0.396213, 0.166476),
(12.5798, 0.502796, 0.186764),
(13.0038, 0.515881, 0.230914),
(13.56, 0.868406, 0.27976),
(14.2426, 1.08123, 0.35042),
(14.8946, 1.30425, 0.0910588),
(15.6624, 1.73194, 0.298224),
(16.4002, 1.92628, 0.0461697),
(17.4466, 2.34611, 0.246344),
(18.6338, 2.5679, 0.372043),
(19.681, 2.80164, 0.380719),
(20.9761, 3.19251, 0.234916),
(22.2597, 3.59141, 0.193237),
(23.6069, 3.93697, 0.289224),
(24.9147, 4.36528, 0.287164),
(25.931, 4.31573, 0.143315),
(26.8196, 4.15128, 0.276521),
(27.7994, 4.36525, 0.119292),
(28.5455, 4.28185, -0.228823),
(29.3201, 3.92544, -0.52463),
(30.2061, 3.15374, -0.666145),
(30.9685, 2.09677, -0.880368),
(31.7091, 0.577643, -1.15718),
(32.1993, -1.58524, -1.36414),
(31.9088, -3.96543, -1.79413),
(31.1956, -5.74699, -1.99003),
(29.8909, -7.7801, -2.12717),
(28.5273, -9.94522, -2.16253),
(26.7218, -11.859, -2.4126),
(24.6321, -12.8463, -2.74501),
(22.9066, -13.4419, -2.80506),
(21.7376, -13.8619, -2.79265),
(20.8446, -14.2394, -2.75015),
(20.2351, -14.5273, -2.7181),
(19.078, -15.1421, -2.65578),
(17.572, -16.0903, -2.57114),
(15.8787, -17.391, -2.46957),
(14.0307, -19.1717, -2.34746),
(12.1297, -21.3388, -2.32964),
(9.13477, -22.7199, -2.78052),
(5.59532, -23.62, -2.83935),
(2.15553, -24.253, -3.08863),
(-0.265872, -23.5511, 2.75214),
(-2.18596, -21.7581, 2.31388),
(-3.57928, -18.27, 1.74186),
(-3.63818, -14.5159, 1.54511),
(-3.11631, -10.5775, 1.3888),
(-2.2527, -7.09292, 1.3374),
(-1.51859, -3.45682, 1.35866),
(-0.471021, -1.0067, 1.03468),
(0.868115, 0.162783, 0.68049),
(3.00994, 0.911998, 0.297343),
(6.59833, 1.79717, 0.221901),
(7.95755, 2.06723, 0.196447),
(8.03836, 2.08216, 0.194939)])

plt.scatter(a[:,0],a[:,1])
plt.show()