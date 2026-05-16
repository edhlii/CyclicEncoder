const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const os = require('os');

const app = express();
const PORT = 3000;

app.use(express.static('public'));
app.use(express.json());

app.post('/api/encode', (req, res) => {
    const { n, k, g, m } = req.body;
    
    // Basic validation
    if (!n || !k || !g || !m) {
        return res.status(400).json({ error: 'Missing parameters' });
    }
    if (g.length !== (n - k + 1)) {
        return res.status(400).json({ error: `g(x) length must be n-k+1 = ${n-k+1}` });
    }
    if (m.length !== k) {
        return res.status(400).json({ error: `m(x) length must be k = ${k}` });
    }

    const exeName = os.platform() === 'win32' ? 'web_encoder.exe' : 'web_encoder';
    const exePath = path.join(__dirname, '..', exeName);
    const command = `"${exePath}" ${n} ${k} ${g} ${m}`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing C++ program: ${error}`);
            return res.status(500).json({ error: 'Server error during encoding' });
        }
        if (stderr) {
            console.error(`C++ program stderr: ${stderr}`);
        }
        
        try {
            const result = JSON.parse(stdout);
            // Append the original polynomials for frontend to use
            result.g_str = g;
            result.m_str = m;
            res.json(result);
        } catch (e) {
            res.status(500).json({ error: 'Failed to parse encoder output' });
        }
    });
});

app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});