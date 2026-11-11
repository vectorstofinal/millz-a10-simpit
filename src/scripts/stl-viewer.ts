// <stl-viewer src="/files/x.stl" label="Dimmer knob v6"></stl-viewer>
// Drag to orbit, scroll/pinch to zoom. Loads three.js only on pages that use it.
import * as THREE from 'three';
import { STLLoader } from 'three/examples/jsm/loaders/STLLoader.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

class StlViewer extends HTMLElement {
  private started = false;
  connectedCallback() {
    if (this.started) return;
    this.started = true;
    const label = this.getAttribute('label');
    if (label) { const l = document.createElement('div'); l.className = 'lbl'; l.textContent = label + ' · drag to orbit'; this.appendChild(l); }
    const msg = document.createElement('div'); msg.className = 'msg'; msg.textContent = 'Loading model…'; this.appendChild(msg);
    const io = new IntersectionObserver((es) => { if (es.some((e) => e.isIntersecting)) { io.disconnect(); this.init(msg); } }, { rootMargin: '200px' });
    io.observe(this);
  }
  private init(msg: HTMLElement) {
    const src = this.getAttribute('src')!;
    const color = this.getAttribute('color') || '#c9cdc9';
    const renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
    renderer.setPixelRatio(Math.min(devicePixelRatio, 2));
    this.prepend(renderer.domElement);
    const scene = new THREE.Scene();
    const camera = new THREE.PerspectiveCamera(35, 1, 0.1, 5000);
    scene.add(new THREE.HemisphereLight(0xffffff, 0x334433, 1.4));
    const key = new THREE.DirectionalLight(0xffffff, 2.2); key.position.set(1, 1.6, 1.2); scene.add(key);
    const rim = new THREE.DirectionalLight(0x9effb4, 0.8); rim.position.set(-1.2, 0.4, -1); scene.add(rim);
    const controls = new OrbitControls(camera, renderer.domElement);
    controls.enableDamping = true;
    const resize = () => {
      const w = this.clientWidth, h = this.clientHeight;
      renderer.setSize(w, h, false); camera.aspect = w / h; camera.updateProjectionMatrix();
    };
    new ResizeObserver(resize).observe(this); resize();
    new STLLoader().load(src, (geo) => {
      geo.computeVertexNormals();
      geo.center();
      const mesh = new THREE.Mesh(geo, new THREE.MeshStandardMaterial({ color, roughness: 0.55, metalness: 0.05 }));
      mesh.rotation.x = -Math.PI / 2; // STL Z-up -> three Y-up
      scene.add(mesh);
      geo.computeBoundingSphere();
      const r = geo.boundingSphere!.radius;
      camera.position.set(r * 1.6, r * 1.5, r * 2.2);
      camera.near = r / 100; camera.far = r * 100; camera.updateProjectionMatrix();
      controls.target.set(0, 0, 0); controls.update();
      msg.remove();
    }, undefined, () => { msg.textContent = 'Could not load the model.'; });
    const loop = () => { controls.update(); renderer.render(scene, camera); requestAnimationFrame(loop); };
    loop();
  }
}
customElements.define('stl-viewer', StlViewer);
